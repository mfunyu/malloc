#include "ft_printf.h"
#include "malloc.h"
#include <unistd.h>
#include <sys/mman.h>


t_malloc zones;

size_t	align_size(size_t size)
{
	return ((size + ALIGNMENT - 1) / ALIGNMENT + 1);
}

void	*alloc_pages_by_size(size_t map_size)
{
	void	*ptr;

	ptr = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (ptr == MAP_FAILED) {
		exit(1);
	}
	return (ptr);
}

size_t	get_map_size(size_t max_block_size)
{
	static int	page_size;
	size_t		map_size;

	if (!page_size) {
		page_size = getpagesize();
		ft_printf("pagesize: %d\n", page_size);
	}
	map_size = page_size * ((max_block_size + ALIGNMENT) * MIN_BLOCKS / page_size);
	ft_printf("mapsize: %d\n", map_size);
	return map_size;
}

void	init_malloc()
{
	zones.initialized = true;
	zones.tiny_zone = alloc_pages_by_size(get_map_size(TINY_MAX));
	zones.small_zone = alloc_pages_by_size(get_map_size(SMALL_MAX));
	ft_printf("%p %p\n", zones.tiny_zone, zones.small_zone);
}

void	*find_block(size_t size)
{
	size_t	aligned_size;
	void	*ptr;

	aligned_size = align_size(size);
	if (!zones.initialized) {
		init_malloc();
	}
	ptr = NULL;
	/*
	if (aligned_size < TINY_MAX) {
		ptr =
	} else if (aligned_size < SMALL_MAX) {
		ptr =
	} else {
		ptr =
	}
	*/
	return (ptr);
}

void	*malloc(size_t size)
{
	void	*ptr;

	ft_printf("malloc called\n");
	ptr = find_block(size);
	return (ptr);
}
