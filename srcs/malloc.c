#include "libft.h"
#include "ft_printf.h"
#include "malloc.h"
#include <sys/mman.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>


void error_exit()
{
	char	*err;

	err = strerror(errno);
	ft_putendl_fd(err, STDERR_FILENO);
	exit(1);
}

void	init_zones()
{
	int		page_size;
	int		map_size;

	page_size = getpagesize();
	map_size = page_size * ((TINY_MAX + DWORD) * 1000 % page_size + 1);
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

void	*malloc(size_t size)
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
