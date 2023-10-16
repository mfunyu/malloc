#include "malloc.h"
#include "lists.h"
#include "init.h"

void	*extend_region(t_magazine *magazine)
{
	t_malloc_chunk	*region;
	t_malloc_chunk	*footer;

	region = init_region(magazine->size);
	if (!region)
		return (NULL);
	footer = (void *)region + magazine->size - REGION_FOOTERSIZE;
	footer->fd = magazine->regions;
	magazine->regions = region;
	lst_malloc_chunk_add_back(&(magazine->freelist), region);
	return (region);
}
