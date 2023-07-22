#ifdef DARWIN
# include <malloc/malloc.h>
#endif
#include "malloc.h"
#include "ft_printf.h"
#include <stddef.h>
#include <stdlib.h>

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

	ft_printf("%p\n", region->freelist);
	if (!region->freelist) {
		region->freelist = chunk;
		PUT(NEXTPTR(chunk), 0);
		PUT(PREVPTR(chunk), 0);
	}
	else {
		void *now = region->freelist;
		void *next = *NEXTPTR(now); 
		while (next) {
			now = next;
			next = *NEXTPTR(now); 
		}
		PUT(PREVPTR(chunk), now);
		PUT(NEXTPTR(chunk), next);
		PUT(NEXTPTR(now), chunk);
	}
	ft_printf("%p\n", region->freelist);
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
	if (ALLOC(chunk)) {
		find_block_and_free(chunk);
	} else {
		exit(1);
	}
}
