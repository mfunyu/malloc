#include "malloc_internal.h"
#include <stdbool.h>

#include "ft_printf.h"  

static bool	_is_allocated_block(t_malloc_chunk *region, t_malloc_chunk *address)
{
	t_malloc_chunk	*chunk;

	chunk = region;
	while (chunk <= address && !IS_FOOTER(chunk))
	{
		if (chunk == address)
			return (IS_ALLOCED(chunk));
		chunk = NEXTCHUNK(chunk);
	}
	return (false);
}

bool	_is_allocated(t_magazine *magazine, t_malloc_chunk *address)
{
	t_malloc_chunk	*footer;
	
	if (magazine->regions <= address && address < magazine->top)
		return (_is_allocated_block(magazine->regions, address));

	footer = (void *)magazine->regions + magazine->size - REGION_FOOTERSIZE;
	for (t_malloc_chunk *region = footer->fd; region; )
	{
		footer = (void *)region + magazine->size - REGION_FOOTERSIZE;
		if (region <= address && address < footer)
			return (_is_allocated_block(region, address));
		region = footer->fd;
	}
	return (false);
}