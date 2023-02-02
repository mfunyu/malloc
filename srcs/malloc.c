#include "libft.h"
#include "ft_printf.h"
#include "malloc.h"
#include <sys/mman.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>

static int g_page_size;
static bool g_malloc_is_initialized = false;
static t_malloc_zone	**g_malloc_zones;

static size_t	round_page(size_t size)
{
	int		page_size;

	page_size = g_page_size;
	if (!page_size)
		page_size = getpagesize();

	return (size + page_size - 1) & ~(page_size - 1);
}

static void	*allocate_pages(size_t size)
{
	size_t	allocation_size;
	void	*addr;

	allocation_size = round_page(size);
	if (allocation_size < size)
		error_exit();

	addr = (char *)mmap(0, allocation_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (addr == MAP_FAILED) {
		error_exit();
	}
	return addr;
}

static t_malloc_zone	*create_zone()
{
	t_malloc_zone	*zone;

	zone = allocate_pages(sizeof(t_malloc_zone));
	return zone;
}

void	*zone_malloc(t_malloc_zone *zone, size_t size)
{


}

static void	malloc_initialize(void)
{
	t_malloc_zone	*zone;

	if (g_malloc_is_initialized)
		return ;
	g_malloc_is_initialized = true;
	g_page_size = getpagesize();

	zone = create_zone();
	g_malloc_zones[0] = zone;
}

void error_exit()
{
	char	*err;

	err = strerror(errno);
	ft_putendl_fd(err, STDERR_FILENO);
	exit(1);
}

void	init_zones()
{
	int		map_size;

	ft_printf("%d\n", g_page_size);
	map_size = g_page_size * ((TINY_MAX + DWORD) * 1000 % g_page_size + 1);
	g_tiny_head = (char *)mmap(0, map_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (g_tiny_head == MAP_FAILED) {
		error_exit();
	}
	g_tiny_max = g_tiny_head + map_size;

	map_size = g_page_size * ((SMALL_MAX + DWORD) * 100 % g_page_size + 1);
	g_small_head = (char *)mmap(g_tiny_max + 1, map_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (g_small_head == MAP_FAILED) {
		error_exit();
	}
	g_small_max = g_small_head + map_size;
	g_large_head = g_small_max + 1;
}

char*	alloc_large(size_t size)
{
	static char*	large_tail;
	char	*new_heap;

	if (!large_tail)
		large_tail = g_large_head;
	new_heap = (char *)mmap(large_tail, size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (new_heap == MAP_FAILED) {
		errno = ENOMEM;
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

	if (!tiny_tail && !small_tail)
	{
		init_zones();
		tiny_tail = g_tiny_head;
		small_tail = g_small_head;
	}
	block_size = size + DWORD;

	if (size <= TINY_MAX)
	{
		head = tiny_tail;
		tiny_tail += block_size;
		if (tiny_tail >= g_tiny_max) {
			errno = ENOMEM;
			error_exit();
		}
		return (head + WORD);
	}
	else if (size <= SMALL_MAX)
	{
		head = small_tail;
		small_tail += block_size;
		if (small_tail >= g_small_max) {
			errno = ENOMEM;
			error_exit();
		}
		return (head + WORD);
	}
	return alloc_large(block_size) + WORD;
}

t_malloc_zone	*malloc_default_zone()
{
	if (!g_malloc_is_initialized)
		malloc_initialize();
	return g_malloc_zones[0];
}

void	*malloc_zone_malloc(t_malloc_zone *zone, size_t size)
{
	char*		new_ptr;
	size_t	aligned_size;

	aligned_size = size;
	if (size % DWORD)
		aligned_size += DWORD - size % DWORD;
	new_ptr = find_new_block(aligned_size);
	PUT(HEADER(new_ptr), PACK(aligned_size, 1));
	PUT(FOOTER(new_ptr), PACK(aligned_size, 1));
	alloc_debug(__func__, __builtin_return_address(0));
	ft_printf("size = %d, ret = %p\n", (int)size, new_ptr);
	return new_ptr;
}

void	*malloc(size_t size)
{
	void	*ptr;

	ptr = malloc_zone_malloc(malloc_default_zone(), size);
	if (ptr == NULL) {
		errno = ENOMEM;
	}
	return ptr;
}
