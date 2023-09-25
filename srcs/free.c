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

void	find_block_and_free(t_heap_chunk *chunk)
{
	size_t		size;
	t_region	*region;
	t_heap_chunk *next;
	t_heap_chunk	*prev;

	size = SIZE(chunk);
	if (size < TINY_MAX) {
		region = &g_regions.tiny_region;
	} else if (size < SMALL_MAX) {
		region = &g_regions.small_region;
	} else {
		if (IS_MAPPED(chunk)) {
			munmap(chunk, size);
			return;
		}
	}
	chunk->size &= ~ALLOCED;
	next = NEXTCHUNK(chunk);
	if (next != region->tail && !IS_ALLOCED(next)) { //merge with one after
		chunk->size += SIZE(next);
		freelst_pop(next, &(region->freelist));
		next = NEXTCHUNK(next);
		size = SIZE(chunk);
	}
	if (!IS_PREV_IN_USE(chunk)) { //merge with just before
		prev = (void *)chunk - chunk->prev_size;
		prev->size += SIZE(chunk);
		size = SIZE(prev);
	} else
		add_chunk_to_freelist(chunk, &(region->freelist));
	next->prev_size = size;
	next->size &= ~PREV_IN_USE;
}

void	free(void *ptr)
{
	t_heap_chunk	*chunk;

#ifdef __APPLE__
	malloc_zone_t	*zone;

	zone = malloc_zone_from_ptr(ptr);
	if (zone) {
		malloc_zone_free(zone, ptr);
		return ;
	}
#endif

	if (!ptr)
		return ;
	ft_printf("free called: %p\n", ptr);
	
	chunk = CHUNK(ptr);
	find_block_and_free(chunk);
}
