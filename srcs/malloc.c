#include "libft.h"
#include "ft_printf.h"
#include "malloc.h"
#include <sys/mman.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>

static char *g_heap_head;
static char *g_heap_max;

void print_heap(void)__attribute__((destructor));

void	print_line(size_t len)
{
	size_t	i;

	i = 0;
	while (i++ < len)
		ft_putchar_fd('-', STDOUT_FILENO);
}

void	print_single_line(size_t len)
{
	print_line(12);
	ft_putchar_fd('+', STDOUT_FILENO);
	print_line(4);
	ft_putchar_fd('+', STDOUT_FILENO);
	print_line(len + 2);
	ft_putchar_fd('+', STDOUT_FILENO);
	print_line(4);
	ft_putchar_fd('+', STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void print_heap()
{
	char	*ptr;
	ft_printf("head: %p\n", g_heap_head);
	ft_printf("end : %p\n", g_heap_max);

	ptr = g_heap_head + WORD;
	while (ptr < g_heap_max && SIZE(ptr))
	{
		print_single_line(ft_strlen(ptr));
		ft_printf("%p ", ptr - WORD);
		if (!ALLOC(ptr))
			ft_printf("%s", CYAN);
		ft_printf("| %d ", *HEADER(ptr));
		ft_printf("| %s ", ptr);
		ft_printf("| %d ", *FOOTER(ptr));
		ft_printf("| ");
		ft_printf("%s", RESET);
		ft_printf("(%u -> %u : %p)\n", SIZE(ptr), SIZE(ptr) + DWORD, SIZE(ptr) + DWORD);
		print_single_line(ft_strlen(ptr));
		ptr = NEXTPTR(ptr);
	}
}

void error_exit()
{
	char *err;
	err = strerror(errno);
	ft_putendl_fd(err, STDERR_FILENO);
	exit(1);
}

char *mem_init()
{
	int map_size;

	map_size = getpagesize() * 42;
	// ft_printf("map_size: %d\n", map_size);
	g_heap_head = (char *)mmap(0, map_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (g_heap_head == MAP_FAILED) {
		error_exit();
	}
	g_heap_max = g_heap_head + map_size;
	return g_heap_head;
}

void	extend_heap()
{
	int		map_size;
	char	*new_heap;

	map_size = getpagesize() * 42;
	new_heap = (char *)mmap(g_heap_max, map_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (new_heap == MAP_FAILED) {
		error_exit();
	}
	g_heap_max += map_size;
}

char	*find_new_block(size_t size)
{
	static char*	mem_tail;
	char*			mem_head;
	size_t			block_size;

	block_size = size + DWORD;
	if (!mem_tail)
		mem_tail = mem_init();
	mem_head = mem_tail;
	mem_tail += block_size;
	if (mem_tail > g_heap_max)
		extend_heap();

	return mem_head + WORD;
}

char	*func(size_t size)
{
	char *	new_ptr;
	size_t	aligned_size;

	aligned_size = size;
	if (size % DWORD)
		aligned_size += DWORD - size % DWORD;
	new_ptr = find_new_block(aligned_size);
	PUT(HEADER(new_ptr), PACK(aligned_size, 1));
	PUT(FOOTER(new_ptr), PACK(aligned_size, 1));

	return new_ptr;
}

void *malloc(size_t size)
{
	ft_printf("malloc: size = %d\n", (int)size);
	return func(size);
}
