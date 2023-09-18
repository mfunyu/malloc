#ifdef __APPLE__
# include <malloc/malloc.h>
#endif
#include "malloc.h"
#include "ft_printf.h"
#include <stddef.h>
#include <stdlib.h>

void	freelst_add_front(t_malloc_chunk **lst, t_malloc_chunk *new)
{
	new->fd = *lst;
	new->bk = NULL;
	if (*lst)
		(*lst)->bk = new;
	*lst = new;
}

void	freelst_insert(t_malloc_chunk *prev, t_malloc_chunk *new)
{
	new->bk = prev;
	new->fd = prev->fd;
	if (prev->fd)
		prev->fd->bk = new;
	prev->fd = new;
}

void	add_chunk_to_freelist(t_malloc_chunk *chunk, t_malloc_chunk **freelist)
{
	t_malloc_chunk	*lst;
	
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

void	merge_free_blocks(t_malloc_chunk *chunk)	
{
	(void)chunk;
	//TODO: impl #4
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
	if (!IS_PREV_IN_USE(chunk))
		merge_free_blocks(chunk);
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
	
	chunk = CHUNK(ptr);
	find_block_and_free(chunk);
}
