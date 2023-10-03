#include "ft_printf.h"
#include "malloc.h"
#include "utils.h"
#include "libft.h"
#include "alloc.h"
#include <unistd.h>
#include <stdio.h>

t_malloc	g_regions;

void	split_chunk(t_heap_chunk *chunk, size_t chunk_size)
{
	t_heap_chunk	*next;

	next = (void *)chunk + chunk_size;
	next->size = chunk->size - chunk_size;
	chunk->size = chunk_size | IS_PREV_IN_USE(chunk);
}

void	*find_chunk_from_region(t_region *region, size_t chunk_size)
{
	t_heap_chunk	*chunk;

	chunk = region->freelist;
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

void	*allocate_chunk_from_region(t_region *region, size_t aligned_size)
{
	size_t			chunk_size;
	t_heap_chunk	*chunk;

	chunk_size = aligned_size + HEADER_SIZE;
	if (chunk_size < MINSIZE)
		chunk_size = MINSIZE;
	chunk = find_chunk_from_region(region, chunk_size);
	if (!chunk)
		return (NULL);
	if (CHUNKSIZE(chunk) - MINSIZE > chunk_size)
	{
		split_chunk(chunk, chunk_size);
		freelst_replace(chunk, NEXTCHUNK(chunk), &(region->freelist));
	}
	else
		freelst_pop(chunk, &(region->freelist));
	chunk->size |= ALLOCED;
	NEXTCHUNK(chunk)->size |= PREV_IN_USE;
	return (MEM(chunk));
}

void	*allocate_chunk_from_heap(t_mmap_chunk **head, size_t size)
{
	t_mmap_chunk	*chunk;
	t_mmap_chunk	*lst;
	int				page_size;

	page_size = get_page_size();
	if (page_size == ERROR)
		return (NULL);
	size = align(size, page_size);
	chunk = map_pages_by_size(size);
	if (!chunk)
		return (NULL);
	chunk->fd = NULL;
	chunk->size = size | MAPPED;
	lst = *head;
	if (!*head)
		*head = chunk;
	else
	{
		while (lst && lst->fd)
			lst = lst->fd;
		lst->fd = chunk;
	}
	return (MEM(chunk));
}

void	*allocate_chunk(size_t size)
{
	size_t	aligned_size;

	aligned_size = align(size, MALLOC_ALIGNMENT);
	if (aligned_size <= TINY_MAX)
		return (allocate_chunk_from_region(&(g_regions.tiny_region), aligned_size));
	else if (aligned_size <= SMALL_MAX)
		return (allocate_chunk_from_region(&(g_regions.small_region), aligned_size));
	return (allocate_chunk_from_heap(&(g_regions.large_lst), size));
}
