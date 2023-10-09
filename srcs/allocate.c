#include "malloc.h"
#include "utils.h"
#include "ft_printf.h"

static void	*_find_unused_chunk(t_magazine *magazine, size_t chunk_size)
{
	t_malloc_chunk	*chunk;

	chunk = magazine->freelist;
	if (!chunk)
	{
		ft_printf("error not enough space\n");
		return (NULL);
	}
	while (chunk->fd && CHUNKSIZE(chunk) < chunk_size)
		chunk = chunk->fd;
	if (CHUNKSIZE(chunk) < chunk_size)
	{
		ft_printf("error not enough space\n");
		return (NULL);
	}
	return (chunk);
}

static void	*_allocate_malloc(t_magazine *magazine, size_t size)
{
	size_t			chunk_size;
	t_malloc_chunk	*chunk;
	t_malloc_chunk	*next;

	chunk_size = align_malloc(size);
	chunk = _find_unused_chunk(magazine, chunk_size);
	if (!chunk)
		return (NULL);
	if (CHUNKSIZE(chunk) - chunk_size > MIN_CHUNKSIZE)
	{
		next = split_chunk(chunk, chunk_size);
		lst_malloc_chunk_replace(&(magazine->freelist), chunk, next);
	}
	else
		lst_malloc_chunk_pop(&(magazine->freelist), chunk);
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
