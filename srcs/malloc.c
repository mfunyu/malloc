#include "ft_printf.h"
#include "malloc.h"
#include "utils.h"
#include "alloc.h"
#include <unistd.h>
#include <stdio.h>

/*
[chunk utilise]
   chunk-> + ----------------------+ -------
           | size of prev chunk    | 8     ↑
           + ----------------------+       |
           | size              | P | 8     |chunk
     ptr-> + ----------------------+ ===== |
           |                       |     ↑ |
           | 〜〜〜 alloced 〜〜〜   |  ptr| |
   	       |                       |     | ↓
nxtchunk-> + ----------------------+ ----|--
           |                       | 8   ↓
           + ----------------------+ =====

[chunk free]
   chunk-> + ----------------------+ -------
           | size of prev chunk    | 8     ↑
           + ----------------------+       |
           | size              | P | 8     |chunk
           + ----------------------+       |
           | nextptr of free-lst   | 8     |
           + ----------------------+       |
		   | prevptr of free-lst   | 8     |
           + ----------------------+       |
           |                       |       |
           | 〜〜〜 alloced 〜〜〜   |       |
   	       |                       |       ↓
nxtchunk-> + ----------------------+ -------
           | size of prev chunk    | 8
           + ----------------------+
*/

t_malloc	g_regions;

void	resize_chunk(t_region *region, t_heap_chunk *chunk, size_t chunk_size)
{
	t_heap_chunk	*next;

	next = (void *)chunk + chunk_size;
	next->size = (chunk->size - chunk_size) | PREV_IN_USE;
	chunk->size = chunk_size | IS_PREV_IN_USE(chunk);
	freelst_replace(chunk, next, &(region->freelist));
}

void	*find_chunk_from_region(t_region *region, size_t chunk_size)
{
	t_heap_chunk	*chunk;

	chunk = region->freelist;
	while (chunk->fd && SIZE(chunk) < chunk_size) {
		chunk = chunk->fd;
	}
	if (SIZE(chunk) < chunk_size) {
		ft_printf("error not enough space\n");
		return NULL;
	}
	return (chunk);
}

void	*allocate_chunk_from_region(t_region *region, size_t size)
{
	t_heap_chunk	*chunk;
	t_heap_chunk	*next;
	size_t			chunk_size;

	chunk_size = align_chunk_size(size);
	chunk = find_chunk_from_region(region, chunk_size);
	if (!chunk)
		return (NULL);

	if (SIZE(chunk) > chunk_size) {
		resize_chunk(region, chunk, chunk_size);
	} else {
		freelst_pop(chunk, &(region->freelist));
	}
	chunk->size |= ALLOCED;
	next = NEXTCHUNK(chunk);
	next->size |= PREV_IN_USE;
	return (MEM(chunk));
}

void	*allocate_chunk_from_heap(size_t size)
{
	t_heap_chunk	*chunk;
	int				page_size;

	page_size = get_page_size();
	if (page_size == ERROR)
		return (NULL);
	size = align(size, page_size);
	chunk = alloc_pages_by_size(size);
	chunk->size = size | MAPPED;
	return (MEM(chunk));
}

void	*allocate_chunk(size_t size)
{
	size_t	aligned_size;

	aligned_size = align_size(size);
	if (aligned_size < TINY_MAX)
		return (allocate_chunk_from_region(&(g_regions.tiny_region), aligned_size));
	else if (aligned_size < SMALL_MAX)
		return (allocate_chunk_from_region(&(g_regions.small_region), aligned_size));
	return (allocate_chunk_from_heap(size));
}

void	*malloc(size_t size)
{
	ft_printf("malloc called %d\n", size);
	if (!size || size > MALLOC_ABSOLUTE_SIZE_MAX)
		return (NULL);
	if (!g_regions.initialized && init_malloc() == ERROR)
		return (NULL);

	return (allocate_chunk(size));
}
