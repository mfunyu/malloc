#include "malloc_internal.h"
#include "init.h"

void	*extend_region(t_magazine *magazine)
{
	t_malloc_chunk	*region;
	t_malloc_chunk	*footer;

	region = init_region(magazine->size);
	if (!region)
		return (NULL);
	footer = (void *)region + magazine->size - REGION_FOOTERSIZE;
	footer->next = magazine->regions;
	magazine->regions = region;
	magazine->top = region;
	return (region);
}
