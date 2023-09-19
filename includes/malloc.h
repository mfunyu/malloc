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

#define TINY_MAX 1008
#define SMALL_MAX 127 * 8192
#define MIN_BLOCKS 100

#define HEADER_SIZE BYTE
# define MINSIZE 24
# define ALLOCED 1
# define PREV_IN_USE 2

# define PUT(ptr, value) *ptr = value
# define SIZE(chunk) (chunk->size & ~PREV_IN_USE & ~ALLOCED)
# define IS_PREV_IN_USE(chunk) (chunk->size & PREV_IN_USE)
# define IS_ALLOCED(chunk) (chunk->size & ALLOCED)
# define ALLOC(ptr, value) *(unsigned int *)(ptr + BYTE) = value

# define MEM(chunk) (void *)chunk + WORD
# define CHUNK(mem) mem - WORD
# define NEXTCHUNK(chunk) ((void *)chunk + SIZE(chunk))
# define PREVPTR(ptr) (unsigned int **)(ptr + WORD + BYTE)

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


void	freelst_add_front(t_malloc_chunk **lst, t_malloc_chunk *new);
void	freelst_insert(t_malloc_chunk *prev, t_malloc_chunk *new);
void	freelst_replace(t_malloc_chunk *old, t_malloc_chunk *new);
void	freelst_pop(t_malloc_chunk *lst, t_malloc_chunk **head);

#endif
