#include "malloc_internal.h"
#include "utils.h"
#include "freelist.h"
#include "ft_printf.h"

static void	*_handle_not_enough_space(t_magazine *magazine)
{
# ifdef BONUS
	return (extend_region(magazine));
# else
	(void)magazine;
	ft_printf("error not enough space\n");
	return (NULL);
# endif
}

static void	*_find_unused_chunk(t_magazine *magazine, size_t chunk_size)
{
	t_malloc_chunk	*chunk;

	chunk = magazine->top;
	if (chunk && !IS_ALLOCED(chunk))
	{
		if (CHUNKSIZE(chunk) >= chunk_size)
		{
			magazine->top = remaindering(chunk, chunk_size, magazine->type);
			return (chunk);
		}
		freelist_add(magazine->freelist, magazine->top);
	}
	chunk = _handle_not_enough_space(magazine);
	if (!chunk)
		return (NULL);
	magazine->top = remaindering(chunk, chunk_size, magazine->type);
	return (chunk);
}

static void	*_mark_allocate(t_malloc_chunk *chunk)
{
	t_malloc_chunk	*next;

	next = NEXTCHUNK(chunk);
	chunk->size |= ALLOCED;
	next->size |= PREV_IN_USE;
	return (MEM(chunk));
}

static void	*_allocate_tiny_malloc(size_t size)
{
	size_t			chunk_size;
	t_magazine		*magazine;
	t_malloc_chunk	*chunk;

	chunk_size = align_malloc(size, TINY);
	magazine = &(g_malloc.tiny_magazine);
	chunk = freelist_takeout(magazine, chunk_size);
	if (chunk)
		return (_mark_allocate(chunk));
	chunk = _find_unused_chunk(magazine, chunk_size);
	if (!chunk)
		return (NULL);
	return (_mark_allocate(chunk));
}

static void	*_allocate_small_malloc(size_t size)
{
	size_t			chunk_size;
	t_magazine		*magazine;
	t_malloc_chunk	*chunk;

	chunk_size = align_malloc(size, SMALL);
	magazine = &(g_malloc.small_magazine);
	chunk = freelist_takeout(magazine, chunk_size);
	if (chunk)
		return (_mark_allocate(chunk));
	chunk = _find_unused_chunk(magazine, chunk_size);
	if (!chunk)
		return (NULL);
	return (_mark_allocate(chunk));
}

static void	*_allocate_large_mmap(t_mmap_chunk **large_allocs, size_t size)
{
	size_t			aligned_size;
	t_mmap_chunk	*chunk;

	aligned_size = align_malloc(size, LARGE);
	if (!aligned_size)
		return (NULL);
	chunk = mmap_by_size(aligned_size);
	if (!chunk)
		return (NULL);
	chunk->fd = *large_allocs;
	chunk->size = aligned_size;
	chunk->size |= MAPPED;
	*large_allocs = chunk;
	return (MEM(chunk));
}

void	*allocate(size_t size)
{
	if (size <= TINY_MAX)
		return (_allocate_tiny_malloc(size));
	else if (size <= SMALL_MAX)
		return (_allocate_small_malloc(size));
	return (_allocate_large_mmap(&(g_malloc.large_allocations), size));
}
