#ifdef __APPLE__
#include <malloc/malloc.h>
#endif
#ifdef BONUS
# include "debug.h"
#endif
#include "malloc_internal.h"
#include "freelist.h"
#include "utils.h"
#include <sys/mman.h>

static bool	_is_allocated_block(t_malloc_chunk *region, t_malloc_chunk *address)
{
	t_malloc_chunk	*chunk;

	chunk = region;
	while (chunk >= address && !IS_FOOTER(chunk))
	{
		if (chunk == address)
			return (IS_ALLOCED(chunk));
		chunk = NEXTCHUNK(chunk);
	}
	return (false);
}

static bool	_is_allocated(t_magazine *magazine, t_malloc_chunk *address)
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

static void	_free_alloc(t_magazine *magazine, t_malloc_chunk *chunk)
{
	t_malloc_chunk	*next;

	if (!_is_allocated(magazine, chunk))
		return (error_msg("pointer being freed was not allocated"));
	chunk->size &= ~ALLOCED;
# ifdef BONUS
	chunk = consolidation(magazine, chunk);
# endif
	if (chunk != magazine->top)
		freelist_add(magazine->freelist, chunk);
	next = NEXTCHUNK(chunk);
	next->prev_size = CHUNKSIZE(chunk);
	next->size &= ~PREV_IN_USE;
}

static void	_free_mmap(t_mmap_chunk **alloced_lst, t_mmap_chunk *chunk)
{
	t_mmap_chunk	*lst;

	if (*alloced_lst == chunk)
		*alloced_lst = chunk->fd;
	else
	{
		lst = *alloced_lst;
		while (lst && lst->fd != chunk)
			lst = lst->fd;
		if (lst->fd == chunk)
			lst->fd = chunk->fd;
		else
			return (error_msg("pointer being freed was not allocated"));
	}
	if (munmap(chunk, CHUNKSIZE(chunk)))
		return (error_msg("munmap failed"));
}

void	free_(void *ptr)
{
	t_malloc_chunk	*chunk;
	size_t			size;

	chunk = CHUNK(ptr);
	if (((uintptr_t)chunk & (TINY_QUANTUM - 1)))
		return (error_msg("Non-aligned pointer being freed"));
	size = CHUNKSIZE(chunk);
	if (IS_MAPPED(chunk))
		_free_mmap(&(g_malloc.large_allocations), (t_mmap_chunk *)chunk);
	else if (size <= TINY_BLOCKSIZE_MAX)
		_free_alloc(&(g_malloc.tiny_magazine), chunk);
	else if (!((uintptr_t)chunk & (SMALL_QUANTUM - 1)))
		_free_alloc(&(g_malloc.small_magazine), chunk);
	else
		return (error_msg("pointer being freed was not allocated"));
}

void	free(void *ptr)
{
	if (!ptr)
		return ;

#ifdef BONUS
	free_debug(__builtin_return_address(0), ptr);
#endif
#ifdef __APPLE__
	malloc_zone_t	*zone;

	zone = malloc_zone_from_ptr(ptr);
	if (zone) {
		malloc_zone_free(zone, ptr);
		return ;
	}
#endif
	free_(ptr);
#ifdef BONUS
	debug_result(NULL);
#endif
}
