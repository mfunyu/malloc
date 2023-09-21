#include "ft_printf.h"
#include "malloc.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

t_malloc	g_regions;

size_t	align(size_t size, size_t align)
{
	return ((size + (align - 1)) & ~(align - 1));
}

size_t	align_size(size_t size)
{
	return align(size, BYTE);
}

size_t align_chunk_size(size_t size)
{
	if (size < MINSIZE)
		size = MINSIZE;
	size += HEADER_SIZE;
	return align(size, MALLOC_ALIGNMENT);
}

void	*alloc_pages_by_size(size_t map_size, void *start)
{
	void	*ptr;

	ptr = mmap(start, map_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (ptr == MAP_FAILED) {
		ft_printf("Error: mmap failed - size %d\n", map_size);
		exit(1);
	}
	return (ptr);
}

size_t	get_map_size(size_t max_block_size)
{
	size_t		map_size;
	static int	page_size;

	if (!page_size) {
		page_size = getpagesize();
		ft_printf("pagesize: %d\n", page_size);
	}
	map_size = page_size * ((max_block_size + MALLOC_ALIGNMENT) * MIN_BLOCKS / page_size);
	ft_printf("mapsize: %d\n", map_size);
	return (map_size);
}

void	init_region(t_region *region, e_size size_type)
{
	size_t 			map_size;
	t_malloc_chunk	*block;

	switch (size_type) {
		case TINY:
			map_size = get_map_size(TINY_MAX);
			break;
		case SMALL:
			map_size = get_map_size(SMALL_MAX);
			break;
		default:
			map_size = 0;
	}
	region->map_size = map_size;
	
	block = (t_malloc_chunk *)alloc_pages_by_size(map_size, NULL);
	block->prev_size = 0;
	block->size = map_size | PREV_IN_USE;
	region->head = block;
	region->tail = (void *)block + map_size;
	add_chunk_to_freelist(block, &region->freelist);
}

void	init_malloc()
{
	g_regions.initialized = true;
	init_region(&(g_regions.tiny_region), TINY);
	init_region(&(g_regions.small_region), SMALL);
	g_regions.large_region.map_size = getpagesize();
	ft_printf("%d\n", g_regions.large_region.map_size);
}

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

void	*find_block_from_region(t_region *region, size_t size)
{
	t_malloc_chunk	*free_chunk;
	t_malloc_chunk 	*next;
	size_t			chunk_size;

	free_chunk = region->freelist;
	chunk_size = align_chunk_size(size);	
	while (free_chunk->fd && SIZE(free_chunk) < chunk_size) {
		free_chunk = free_chunk->fd; 
	}
	if (SIZE(free_chunk) < chunk_size) {
		ft_printf("error not enough space\n");
		return NULL;
	}
	next = free_chunk->fd;
	if (SIZE(free_chunk) > chunk_size) {
		next = (void *)free_chunk + chunk_size;
		freelst_replace(free_chunk, next);
		next->size = (free_chunk->size - chunk_size) | PREV_IN_USE;
		free_chunk->size = chunk_size | IS_PREV_IN_USE(free_chunk);
	} else {
		freelst_pop(free_chunk, &(region->freelist));
	}
	if (free_chunk == region->freelist) {
		region->freelist = next;
	}
	free_chunk->size |= ALLOCED;
	next = NEXTCHUNK(free_chunk);
	next->size |= PREV_IN_USE;
	return (MEM(free_chunk));
}

void	*lagre_block(t_region *region, size_t size)
{
	t_malloc_chunk	*chunk;

	size = align(size, region->map_size);
	chunk = alloc_pages_by_size(size, NULL);
	chunk->size = size | MAPPED;

	return (MEM(chunk));
}

void	*find_block(size_t size)
{
	size_t	aligned_size;
	void	*ptr = NULL;

	if (!size || size > MALLOC_ABSOLUTE_SIZE_MAX)
		return (NULL);
	if (!g_regions.initialized) {
		init_malloc();
	}

	aligned_size = align_size(size);
	ft_printf("size: %d, aligned: %d\n", size, aligned_size);

	if (aligned_size < TINY_MAX) {
		ptr = find_block_from_region(&(g_regions.tiny_region), aligned_size);
	} else if (aligned_size < SMALL_MAX) {
		ft_printf("here\n");
		ptr = find_block_from_region(&(g_regions.small_region), aligned_size);
	} else {
		ptr = lagre_block(&(g_regions.large_region), size);
	}

	return (ptr);
}

void	*malloc(size_t size)
{
	void	*ptr;

	ft_printf("malloc called %d\n", size);
	ptr = find_block(size);
	return (ptr);
}
