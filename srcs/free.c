#ifdef DARWIN
# include <malloc/malloc.h>
#endif
#include "malloc.h"
#include "ft_printf.h"
#include <stddef.h>
#include <stdlib.h>

void	add_chunk_to_freelist(void *chunk, void **freelist)
{
	if (!*freelist || *freelist > chunk) {
		PUT(NEXTPTR(chunk), *freelist);
		PUT(PREVPTR(chunk), 0);
		if (*freelist)
			PUT(PREVPTR(*freelist), chunk);
		*freelist = chunk;
	} else {
		void *now = *freelist;
		void *next = *NEXTPTR(now); 
		while (next && next < chunk) {
			now = next;
			next = *NEXTPTR(now); 
		}
		PUT(PREVPTR(chunk), now);
		PUT(NEXTPTR(chunk), next);
		PUT(NEXTPTR(now), chunk);
		if (next)
			PUT(PREVPTR(next), chunk);
	}
}

void	find_block_and_free(void *chunk)
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
	ALLOC(chunk, 0);
}

void	free(void *ptr)
{
	void	*chunk;

#ifdef DARWIN
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
	if (IS_ALLOCED(chunk)) {
		find_block_and_free(chunk);
	} else {
		exit(1);
	}
}
