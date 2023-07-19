#include "ft_printf.h"
#include "malloc.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>

t_malloc	g_regions;

size_t	align_size(size_t size)
{
	return ((size + (MALLOC_ALIGNMENT - 1)) & ~(MALLOC_ALIGNMENT - 1));
}

void	*alloc_pages_by_size(size_t map_size)
{
	void	*ptr;

	ptr = mmap(0, map_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (ptr == MAP_FAILED) {
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
	size_t map_size;

	switch (size_type) {
		case TINY:
			map_size = TINY_MAX;
			break;
		case SMALL:
			map_size = SMALL_MAX;
			break;
		default:
			map_size = 0;
	}
	region->head = alloc_pages_by_size(get_map_size(map_size));
	region->tail = region->head;
}

void	init_malloc()
{
	g_regions.initialized = true;
	init_region(&(g_regions.tiny_region), TINY);
	init_region(&(g_regions.small_region), SMALL);

	ft_printf("%p %p\n", g_regions.tiny_region.head, g_regions.small_region.head);
}

/*
[chunk utilise]
   chunk-> + ----------------------+
           | size de chunk         | 8
           + ----------------------+
           | empty                 | 8
     mem-> + ----------------------+
           |                       |
   	       |                       |
nxtchunk-> + ----------------------+
           | size de chunk         | 8
           + ----------------------+

[chunk free]
   chunk-> + ----------------------+
           | size of chunk         | 8
           + ----------------------+
           | empty                 | 8
     mem-> + ----------------------+
           | nextptr pour free-lst | 8
           + ----------------------+
		   | prevptr pour free-lst | 8
           + ----------------------+
           |                       |
   	       |                       |
nxtchunk-> + ----------------------+
           | size de chunk         | 8
           + ----------------------+
*/

void	*find_block_from_region(t_region *region, size_t size)
{
	void			*free_chunk;
	unsigned int	**prev;
	unsigned int	**next;
	unsigned int	block_size;
	void			*ptr;

	free_chunk = region->freelist;
	while (free_chunk) {
		block_size = *(unsigned int *)free_chunk;
		if (block_size > size) {
			ptr = free_chunk + BYTE;
			prev = (unsigned int **)ptr;
			next = prev + BYTE;
			unsigned int **prev_ptr = prev + WORD;
			*prev_ptr = *next;
			unsigned int **next_ptr = next + WORD;
			*next_ptr = *prev;
			return (ptr);
		}
		free_chunk = (void *)*((unsigned int**)free_chunk + BYTE);
	}
	ptr = region->tail + BYTE;
	*(unsigned int *)(region->tail) = size;
	region->tail += size + WORD;
	return (ptr);
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
		ptr = find_block_from_region(&(g_regions.small_region), aligned_size);
	} else {
		ptr = g_regions.large_region.head;
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
