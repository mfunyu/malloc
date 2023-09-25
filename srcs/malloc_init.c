#include "malloc.h"
#include "utils.h"
#include "ft_printf.h"
#include "alloc.h"

static int	_init_region(t_region *region, e_size size_type)
{
	size_t 			map_size;
	t_heap_chunk	*block;

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
	
	block = (t_heap_chunk *)alloc_pages_by_size(map_size);
	if (!block)
		return (ERROR);
	block->prev_size = 0;
	block->size = map_size | PREV_IN_USE;
	region->head = block;
	region->tail = (void *)block + map_size;
	add_chunk_to_freelist(block, &region->freelist);
	return (SUCCESS);
}

int	init_malloc()
{
	if (_init_region(&(g_regions.tiny_region), TINY) == ERROR)
		return (ERROR);
	if (_init_region(&(g_regions.small_region), SMALL) == ERROR)
		return (ERROR);
	g_regions.initialized = true;
	return (SUCCESS);
}