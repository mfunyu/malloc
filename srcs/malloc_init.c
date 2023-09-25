#include "malloc.h"
#include "utils.h"
#include "ft_printf.h"
#include "alloc.h"

static void	_init_region(t_region *region, e_size size_type)
{
	size_t 			map_size;
	t_malloc_chunk	*block;

	switch (size_type) {
		case TINY:
			map_size = calc_map_size(TINY_MAX);
			break;
		case SMALL:
			map_size = calc_map_size(SMALL_MAX);
			break;
		default:
			map_size = 0;
	}
	region->map_size = map_size;
	
	block = (t_malloc_chunk *)alloc_pages_by_size(map_size, NULL);
	block->prev_size = 0;
	block->size = map_size | PREV_IN_USE;
	region->head = block;
	region->tail = (void *)block + map_size;
	add_chunk_to_freelist(block, &region->freelist);
}

void	init_malloc()
{
	g_regions.initialized = true;
	_init_region(&(g_regions.tiny_region), TINY);
	_init_region(&(g_regions.small_region), SMALL);
	g_regions.large_region.map_size = getpagesize();
	ft_printf("%d\n", g_regions.large_region.map_size);
}