#include "libft.h"
#include "ft_printf.h"
#include "malloc.h"
#include <sys/mman.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>

static char *g_tiny_head;
static char *g_tiny_max;
static char *g_small_head;
static char *g_small_max;
static char *g_large_head;

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
	ft_printf("head: %p\n", g_tiny_head);
	ft_printf("end : %p\n", g_tiny_max);

	ptr = g_tiny_head + WORD;
	while (ptr < g_tiny_max && SIZE(ptr))
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

void	init_zones()
{
	int		page_size;
	int		map_size;

	page_size = getpagesize();
	map_size = page_size * ((TINY_MAX + DWORD) * 100 % page_size + 1);
	g_tiny_head = (char *)mmap(0, map_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (g_tiny_head == MAP_FAILED) {
		error_exit();
	}
	g_tiny_max = g_tiny_head + map_size;

	map_size = page_size * ((SMALL_MAX + DWORD) * 100 % page_size + 1);
	g_small_head = (char *)mmap(g_tiny_max + 1, map_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (g_small_head == MAP_FAILED) {
		error_exit();
	}
	g_small_max = g_small_head + map_size;
	g_large_head = g_small_max + 1;
}

char*	alloc_large(size)
{
	static char*	large_tail;
	char	*new_heap;

	if (!large_tail)
		large_tail = g_large_head;
	new_heap = (char *)mmap(large_tail, size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (new_heap == MAP_FAILED) {
		error_exit();
	}
	large_tail += size;
	return new_heap;
}

char	*find_new_block(size_t size)
{
	static char*	tiny_tail;
	static char*	small_tail;
	size_t	block_size;
	char*	head;

	if (!tiny_tail && !small_tail) {
		init_zones();
		tiny_tail = g_tiny_head;
		small_tail = g_small_head;
	}
	block_size = size + DWORD;

	if (size <= TINY_MAX) {
		head = tiny_tail;
		tiny_tail += block_size;
		if (tiny_tail >= g_tiny_max)
			error_exit();
		return head + WORD;
	}
	else if (size <= SMALL_MAX) {
		head = small_tail;
		small_tail += block_size;
		if (small_tail >= g_small_max)
			error_exit();
		return head + WORD;
	}
	return alloc_large(block_size) + WORD;
}

void *malloc(size_t size)
{
	ft_printf("malloc: size = %d\n", (int)size);
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
