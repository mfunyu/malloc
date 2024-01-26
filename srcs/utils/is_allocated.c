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

static bool	_is_allocated_magazine(t_magazine *magazine, t_malloc_chunk *address)
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

static bool	_is_allocated_mmap(t_mmap_chunk *allocations, t_malloc_chunk *address)
{
	t_mmap_chunk	*chunk;

	chunk = allocations;
	while (chunk)
	{
		if (CHUNK(address) == chunk)
			return (IS_MAPPED(address));
		chunk = chunk->fd;
	}
	return (false);
}

static bool	_is_allocated(t_malloc_chunk *address, e_size type)
{
	if (type == TINY)
		return (_is_allocated_magazine(&g_malloc.tiny_magazine, address));
	if (type == SMALL)
		return (_is_allocated_magazine(&g_malloc.small_magazine, address));
	if (type == LARGE)
		return (_is_allocated_mmap(g_malloc.large_allocations, address));
	return (false);
}

bool	is_allocated_hint(t_malloc_chunk *address, e_size hint)
{
	static e_size	last_hint = NONE;
 
	if (hint == NONE)
		hint = last_hint;
	last_hint = hint;
	if (hint == TINY)
		return (_is_allocated(address, TINY) || _is_allocated(address, SMALL) || _is_allocated(address, LARGE));
	if (hint == SMALL)
		return (_is_allocated(address, SMALL) || _is_allocated(address, TINY) || _is_allocated(address, LARGE));
	if (hint == LARGE)
		return (_is_allocated(address, LARGE) || _is_allocated(address, SMALL) || _is_allocated(address, TINY));
	return (false);
}