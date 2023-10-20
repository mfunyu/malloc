#include "malloc.h"
#include "utils.h"
#include "freelist.h"
#include "lists.h"
#include "ft_printf.h"

static void	*_handle_not_enough_space(t_magazine *magazine)
{
# ifdef BONUS
	return (extend_region(magazine));
# else
	ft_printf("error not enough space\n");
	return (NULL);
# endif
}

static void	*_find_unused_chunk(t_magazine *magazine, size_t chunk_size)
{
	t_malloc_chunk	*chunk;
	t_malloc_chunk	*next;

	chunk = freelist_takeout(magazine->freelist, chunk_size);
	if (chunk)
		return (chunk);
	chunk = magazine->top;
	if (chunk && !IS_ALLOCED(chunk))
	{
		if (CHUNKSIZE(chunk) >= chunk_size)
		{
			if (CHUNKSIZE(chunk) > MIN_CHUNKSIZE + chunk_size)
			{
				next = split_chunk(chunk, chunk_size);
				magazine->top = next;
			}
			else
				magazine->top = NULL;
			return (chunk);
		}
		freelist_add(magazine->freelist, magazine->top);
	}
	chunk = _handle_not_enough_space(magazine);
	if (!chunk)
		return (NULL);
	if (CHUNKSIZE(chunk) > MIN_CHUNKSIZE + chunk_size)
	{
		next = split_chunk(chunk, chunk_size);
		magazine->top = next;
	}
	else
		magazine->top = NULL;
	return (chunk);
}

static void	*_allocate_malloc(t_magazine *magazine, size_t size)
{
	size_t			chunk_size;
	t_malloc_chunk	*chunk;
	t_malloc_chunk	*next;

	chunk_size = align_malloc_chunk(size);
	chunk = _find_unused_chunk(magazine, chunk_size);
	if (!chunk)
		return (NULL);
	next = NEXTCHUNK(chunk);
	chunk->size |= ALLOCED;
	next->size |= PREV_IN_USE;
	return (MEM(chunk));
}

static void	*_allocate_mmap(t_mmap_chunk **large_allocs, size_t size)
{
	size_t			aligned_size;
	t_mmap_chunk	*chunk;

	aligned_size = align_large(size);
	if (!aligned_size)
		return (NULL);
	chunk = mmap_by_size(aligned_size);
	if (!chunk)
		return (NULL);
	chunk->fd = NULL;
	chunk->size = aligned_size;
	chunk->size |= MAPPED;
	lst_mmap_chunk_add(large_allocs, chunk);
	return (MEM(chunk));
}

void	*allocate(size_t size)
{
	if (size <= TINY_MAX)
		return (_allocate_malloc(&(g_malloc.tiny_magazine), size));
	else if (size <= SMALL_MAX)
		return (_allocate_malloc(&(g_malloc.small_magazine), size));
	return (_allocate_mmap(&(g_malloc.large_allocations), size));
}
