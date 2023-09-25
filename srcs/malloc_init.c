#include "malloc.h"
#include "utils.h"
#include "ft_printf.h"
#include "alloc.h"

static size_t	_init_map_size(t_region *region, e_size size_type)
{
	size_t 			map_size;

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
	return (map_size);
}

static int _init_block(t_region *region)
{
	t_heap_chunk	*block;

	block = (t_heap_chunk *)alloc_pages_by_size(region->map_size);
	if (!block)
		return (ERROR);
	block->prev_size = 0;
	block->size = region->map_size | PREV_IN_USE;
	region->head = block;
	region->tail = (void *)block + region->map_size;
	return (SUCCESS);
}

static int	_init_region(t_region *region, e_size size_type)
{
	if (!_init_map_size(region, size_type))
		return (ERROR);
	
	if (_init_block(region) == ERROR)
		return (ERROR);

	add_chunk_to_freelist(region->head, &region->freelist);
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