#ifdef __APPLE__
# include <malloc/malloc.h>
#endif
#include "malloc.h"
#include "ft_printf.h"
#include <stddef.h>
#include <stdlib.h>
#include <sys/mman.h>

void	add_chunk_to_freelist(t_heap_chunk *chunk, t_heap_chunk **freelist)
{
	t_heap_chunk	*lst;

	if (!*freelist || SIZE((*freelist)) >= SIZE(chunk)) {
		freelst_add_front(freelist, chunk);
	} else {
		lst = *freelist;
		while (lst->fd && SIZE(lst->fd) < SIZE(chunk)) {
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
	if (next != region->tail && !IS_ALLOCED(next))
	{
		chunk->size += SIZE(next);
		freelst_pop(next, &(region->freelist));
	}
	if (!IS_PREV_IN_USE(chunk))
	{
		prev = (void *)chunk - chunk->prev_size;
		prev->size += SIZE(chunk);
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
	next->prev_size = SIZE(chunk);
	next->size &= ~PREV_IN_USE;
}

void	free_chunk(t_heap_chunk *chunk)
{
	size_t	size;

	size = SIZE(chunk);
	if (IS_MAPPED(chunk))
		munmap(chunk, size);
	else if (size < TINY_MAX)
		find_block_and_free(&(g_regions.tiny_region), chunk);
	else if (size < SMALL_MAX)
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
	free_chunk(CHUNK(ptr));
}
