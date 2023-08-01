#ifdef __APPLE__
# include <malloc/malloc.h>
#endif
#include "malloc.h"
#include "ft_printf.h"
#include <stddef.h>
#include <stdlib.h>

void	add_chunk_to_freelist(t_malloc_chunk *chunk, t_malloc_chunk **freelist)
{
	t_malloc_chunk	*head;

	head = *freelist;
	if (!head || SIZE(head) >= SIZE(chunk)) {
		chunk->fd = head;
		chunk->bk = NULL;
		if (head)
			head->bk = chunk;
		*freelist = chunk;
	} else {
		t_malloc_chunk	*now;
		
		now = *freelist;
		while (now->fd && SIZE(now->fd) < SIZE(chunk)) {
			now = now->fd;
		}
		chunk->bk = now;
		chunk->fd = now->fd;
		now->fd = chunk;
		if (now->fd) {
			now->fd->bk = chunk;
		}
	}
}

void	find_block_and_free(t_malloc_chunk *chunk)
{
	size_t		size;
	t_region	*region;

	size = SIZE(chunk);
	if (size <= TINY_MAX) {
		region = &g_regions.tiny_region;
	} else if (size <= SMALL_MAX) {
		region = &g_regions.small_region;
	} else {
		region = &g_regions.large_region;
	}
	add_chunk_to_freelist(chunk, &(region->freelist));
	t_malloc_chunk *next = NEXTCHUNK(chunk);
	next->prev_size = SIZE(chunk);
	next->size &= ~PREV_IN_USE;
}

void	free(void *ptr)
{
	t_malloc_chunk	*chunk;

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
	
	chunk = ptr - WORD;
	find_block_and_free(chunk);
}
