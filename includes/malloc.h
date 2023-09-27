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
# define MAPPED 4

# define PUT(ptr, value) *ptr = value
# define SIZE(chunk) (chunk->size & ~MAPPED & ~PREV_IN_USE & ~ALLOCED)
# define IS_PREV_IN_USE(chunk) (chunk->size & PREV_IN_USE)
# define IS_ALLOCED(chunk) (chunk->size & ALLOCED)
# define IS_MAPPED(chunk) (chunk->size & MAPPED)
# define ALLOC(ptr, value) *(unsigned int *)(ptr + BYTE) = value

# define MEM(chunk) (void *)chunk + WORD
# define CHUNK(mem) mem - WORD
# define NEXTCHUNK(chunk) ((t_heap_chunk *)((void *)chunk + SIZE(chunk)))
# define PREVPTR(ptr) (unsigned int **)(ptr + WORD + BYTE)

typedef enum s_size
{
	TINY,
	SMALL,
	LARGE
}			e_size;

typedef struct s_heap_chunk
{
	size_t					prev_size;
	size_t					size;
	struct s_heap_chunk 	*fd;
	struct s_heap_chunk 	*bk;
}				t_heap_chunk;

typedef struct s_mmap_chunk
{
	struct s_mmap_chunk	*fd;
	size_t				size;
}				t_mmap_chunk;

typedef struct s_region
{
	size_t			map_size;
	void			*tail;
	t_heap_chunk	*head;
	t_heap_chunk	*freelist;
}				t_region;

typedef struct s_malloc
{
	bool		initialized;
	t_region	tiny_region;
	t_region	small_region;
	t_mmap_chunk	*large_lst;
}			t_malloc;

extern t_malloc	g_regions;

void	show_alloc_heap();
void	show_alloc_mem();
void	show_free_list(t_region region);
void	add_chunk_to_freelist(t_heap_chunk *chunk, t_heap_chunk **freelist);

int		init_malloc();

void	freelst_add_front(t_heap_chunk **lst, t_heap_chunk *new_chunk);
void	freelst_insert(t_heap_chunk *prev, t_heap_chunk *new_chunk);
void	freelst_replace(t_heap_chunk *old, t_heap_chunk *new_chunk, t_heap_chunk **head);
void	freelst_pop(t_heap_chunk *lst, t_heap_chunk **head);

#endif
