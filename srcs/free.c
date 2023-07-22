#include "malloc.h"
#include "ft_printf.h"
#include <stddef.h>
#include <stdlib.h>
#ifdef HOST_ARCH
#include <malloc/malloc.h>
#endif

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
		ft_printf("here\n");
		void *prev = region->freelist;
		void *next = (void *)*((unsigned int**)region->freelist + WORD);
		while (next && next < chunk) {
				
			next = (void *)*((unsigned int**)next + WORD);
			prev = (void *)*((unsigned int**)prev + WORD);
		}
		ft_printf("here\n");
		unsigned int **prev_next = (unsigned int **)prev + WORD;
		*prev_next = chunk;
		if (next) {
			unsigned int **next_prev = (unsigned int **)next + WORD + BYTE;
			*next_prev = chunk;
		}
		ft_printf("here\n");
		unsigned int **chunk_next = (unsigned int **)chunk + WORD;
		*chunk_next = next;
		unsigned int **chunk_prev = (unsigned int **)chunk + WORD + BYTE;
		*chunk_prev = prev;
		unsigned int **chunk_alloc = (unsigned int **)chunk + BYTE;
		*chunk_alloc = 0;
	}
	ft_printf("%p\n", region->freelist);
}

void	free(void *ptr)
{
	void	*chunk;

#ifdef HOST_ARCH
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
