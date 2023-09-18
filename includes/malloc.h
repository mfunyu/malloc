#ifndef MALLOC_H
# define MALLOC_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define APPROX_PAGE_SIZE 4096
#define MALLOC_ABSOLUTE_SIZE_MAX (SIZE_MAX - (2 * APPROX_PAGE_SIZE))

#define MALLOC_ALIGNMENT 16
#define BYTE 8
#define WORD 16

#define ALLOCED 1

#define TINY_MAX 1008
#define SMALL_MAX 127 * 8192
#define MIN_BLOCKS 100

# define MINSIZE 24

# define PUT(ptr, value) *ptr = value
# define SIZE(chunk) (chunk->size & ~0x1)
# define IS_PREV_IN_USE(chunk) (chunk->size & 0x1)
# define ALLOC(ptr, value) *(unsigned int *)(ptr + BYTE) = value

# define MEM(chunk) (void *)chunk + WORD
# define CHUNK(mem) mem - WORD
# define NEXTCHUNK(chunk) ((void *)chunk + SIZE(chunk))
# define PREVPTR(ptr) (unsigned int **)(ptr + WORD + BYTE)
# define PREV_IN_USE 1

typedef enum s_size
{
	TINY,
	SMALL,
	LARGE
}			e_size;

typedef struct s_malloc_chunk
{
	size_t					prev_size;
	size_t					size;
	struct s_malloc_chunk 	*fd; 
	struct s_malloc_chunk 	*bk; 
}				t_malloc_chunk;

typedef struct s_region
{
	size_t			map_size;
	void			*tail;
	t_malloc_chunk	*blocks;
	t_malloc_chunk	*freelist;
}				t_region;

typedef struct s_malloc
{
	bool		initialized;
	t_region	tiny_region;
	t_region	small_region;
	t_region	large_region;
}			t_malloc;

extern t_malloc	g_regions;

void	show_alloc_heap();
void	show_free_list(t_region region);
void	add_chunk_to_freelist(t_malloc_chunk *chunk, t_malloc_chunk **freelist);

#endif
