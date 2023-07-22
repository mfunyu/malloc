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

	size = *(unsigned int *)chunk;
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
		unsigned int **chunk_next = (unsigned int **)chunk + WORD;
		*chunk_next = 0;
		unsigned int **chunk_prev = (unsigned int **)chunk + WORD + BYTE;
		*chunk_prev = 0;
	}
	else {
		unsigned int *now = region->freelist;
		unsigned int *next = *((unsigned int **)now + WORD); 
		while (next) {
			now = next;
			next = *((unsigned int **)now + WORD); 
		}
		ft_printf("now: %p\n", now);
		ft_printf("next: %p\n", next);
		unsigned int **chunk_prev = (unsigned int **)chunk + WORD + BYTE;
		*chunk_prev = now;
		unsigned int **chunk_next = (unsigned int **)chunk + WORD;
		*chunk_next = next;
		unsigned int **now_next = (unsigned int **)now + WORD;
		*now_next = chunk;

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
	if (*(unsigned int *)(chunk + BYTE)) {
		find_block_and_free(chunk);
	} else {
		exit(1);
	}
}
