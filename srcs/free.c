#ifdef __APPLE__
# include <malloc/malloc.h>
#endif
#include "malloc.h"
#include "ft_printf.h"
#include <stddef.h>
#include <sys/mman.h>

void	add_chunk_to_freelist(t_heap_chunk *chunk, t_heap_chunk **freelist)
{
	t_heap_chunk	*lst;

	if (!*freelist || CHUNKSIZE((*freelist)) >= CHUNKSIZE(chunk)) {
		freelst_add_front(freelist, chunk);
	} else {
		lst = *freelist;
		while (lst->fd && CHUNKSIZE(lst->fd) < CHUNKSIZE(chunk)) {
			lst = lst->fd;
		}
		freelst_insert(lst, chunk);
	}
}

void	*merge_chunks(t_heap_chunk *chunk, t_region *region)
{
	t_heap_chunk	*next;
	t_heap_chunk	*prev;

	next = NEXTCHUNK(chunk);
	if (!IS_FOOTER(chunk) && !IS_ALLOCED(next))
	{
		chunk->size += CHUNKSIZE(next);
		freelst_pop(next, &(region->freelist));
	}
	if (!IS_PREV_IN_USE(chunk))
	{
		prev = (void *)chunk - chunk->prev_size;
		prev->size += CHUNKSIZE(chunk);
		freelst_pop(prev, &(region->freelist));
		return (prev);
	}
	return (chunk);
}

void	find_block_and_free(t_region *region, t_heap_chunk *chunk)
{
	t_heap_chunk	*next;

	chunk = merge_chunks(chunk, region);
	add_chunk_to_freelist(chunk, &(region->freelist));
	chunk->size &= ~ALLOCED;
	next = NEXTCHUNK(chunk);
	next->prev_size = CHUNKSIZE(chunk);
	next->size &= ~PREV_IN_USE;
}

void	free_mmapped_block(t_mmap_chunk **head, t_mmap_chunk *chunk)
{
	t_mmap_chunk	*lst;

	if (*head == chunk)
	{
		*head = chunk->fd;
		return ;
	}
	lst = *head;
	while (lst && lst->fd != chunk)
		lst = lst->fd;
	if (lst->fd == chunk)
		lst->fd = chunk->fd;
	munmap(chunk, CHUNKSIZE(chunk));
}

void	free_(void *ptr)
{
	t_heap_chunk	*chunk;
	size_t			size;

	chunk = CHUNK(ptr);
	size = ALLOCSIZE(chunk);
	if (IS_MAPPED(chunk))
		free_mmapped_block(&(g_regions.large_lst), (t_mmap_chunk *)chunk);
	else if (size <= TINY_MAX)
		find_block_and_free(&(g_regions.tiny_region), chunk);
	else if (size <= SMALL_MAX)
		find_block_and_free(&(g_regions.small_region), chunk);
}

void	free(void *ptr)
{
#ifdef __APPLE__
	malloc_zone_t	*zone;

	zone = malloc_zone_from_ptr(ptr);
	if (zone) {
		malloc_zone_free(zone, ptr);
		return ;
	}
#endif

	ft_printf("free called: %p\n", ptr);
	if (!ptr)
		return ;
	free_(ptr);
}
