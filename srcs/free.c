#include "malloc.h"
#include "utils.h"

static t_malloc_chunk	*_merge_chunks(t_magazine *magazine, t_malloc_chunk *chunk)
{
	t_malloc_chunk	*next;
	t_malloc_chunk	*prev;

	chunk->size &= ~ALLOCED;
	if (!IS_ALLOCED(NEXTCHUNK(chunk)))
	{
		next = NEXTCHUNK(chunk);
		chunk->size += CHUNKSIZE(next);
		lst_malloc_chunk_pop(&(magazine->freelist), next);
	}
	if (!IS_PREV_IN_USE(chunk))
	{
		prev = PREVCHUNK(chunk);
		prev->size += CHUNKSIZE(chunk);
		lst_malloc_chunk_pop(&(magazine->freelist), prev);
		chunk = prev;
	}
	return (chunk);
}

static void	_free_alloc(t_magazine *magazine, t_malloc_chunk *chunk)
{
	t_malloc_chunk	*next;

	chunk = _merge_chunks(magazine, chunk);
	lst_malloc_chunk_sort_add(&(magazine->freelist), chunk);
	next = NEXTCHUNK(chunk);
	next->prev_size = CHUNKSIZE(chunk);
	next->size &= ~PREV_IN_USE;
}

static void	_free_mmap(t_mmap_chunk *alloced_lst, t_mmap_chunk *chunk)
{
	lst_mmap_chunk_pop(alloced_lst, chunk);
	if (munmap(chunk, CHUNKSIZE(chunk)))
	{
		ft_printf("Error: munmap\n");
		return ;
	}
}

void	free_(void *ptr)
{
	t_malloc_chunk	*chunk;
	size_t			size;

	chunk = CHUNK(ptr);
	size = ALLOCSIZE(chunk);
	if (IS_MAPPED(chunk))
		_free_mmap(&(g_malloc.large_allocations), chunk);
	else if (size <= TINY_MAX)
		_free_alloc(&(g_malloc.tiny_magazine), chunk);
	else if (size <= SMALL_MAX)
		_free_alloc(&(g_malloc.small_magazine), chunk);
}

void	free(void *ptr)
{
	if (!ptr)
		return ;

#ifdef __APPLE__
	malloc_zone_t	*zone;

	zone = malloc_zone_from_ptr(ptr);
	if (zone) {
		malloc_zone_free(zone, ptr);
		return ;
	}
#endif
	free_(ptr);
}
