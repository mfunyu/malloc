#ifndef MALLOC_INTERNAL_H
# define MALLOC_INTERNAL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

# ifdef BONUS
#  include "bonus.h"
# endif

# define MALLOC_ALIGNMENT 16
# define TINY_QUANTUM MALLOC_ALIGNMENT /* 16 */
# define SMALL_QUANTUM 512
# define NUMS_TINY_SLOTS 64
# define TINY_MAX ((NUMS_TINY_SLOTS) * TINY_QUANTUM) /* 1024 */
# define SMALL_MAX 127 * 8192 /* 1040384 */
# define MIN_ALLOCNUMS 100

# define APPROX_PAGE_SIZE 4096
# define MALLOC_ABSOLUTE_SIZE_MAX (SIZE_MAX - (2 * APPROX_PAGE_SIZE))

# define ALIGN(size, align) ((size + (align - 1)) & ~(align - 1))

# define MIN_CHUNKSIZE ALIGN(CHUNK_HEADERSIZE + 1, TINY_QUANTUM) /* 32 */
# define TINY_BLOCKSIZE_MAX ALIGN(TINY_MAX + CHUNK_OVERHEAD, TINY_QUANTUM) + TINY_QUANTUM /* 1056 */
# define SMALL_BLOCKSIZE_MAX ALIGN(SMALL_MAX + CHUNK_OVERHEAD, SMALL_QUANTUM)

# define CHUNK_OVERHEAD 8
# define CHUNK_HEADERSIZE 16
# define REGION_FOOTERSIZE ALIGN(sizeof(t_malloc_footer), MALLOC_ALIGNMENT)
# define LARGE_HEADERSIZE sizeof(t_mmap_chunk)

# define CHUNK(ptr) ((void *)ptr - CHUNK_HEADERSIZE)
# define MEM(chunk) ((void *)chunk + CHUNK_HEADERSIZE)
# define IS_PREV_IN_USE(chunk) (chunk->size & PREV_IN_USE)
# define IS_ALLOCED(chunk) (chunk->size & ALLOCED)
# define IS_MAPPED(chunk) (chunk->size & MAPPED)
# define GET_FLAGS(chunk) (chunk->size & (ALL - 1))
# define IS_FOOTER(chunk) (CHUNKSIZE(chunk) == 0 && IS_ALLOCED(chunk))

# define CHUNKSIZE(chunk) (chunk->size & ~(ALL - 1))
# define ALLOCSIZE(chunk) (CHUNKSIZE(chunk) - CHUNK_OVERHEAD)
# define NEXTCHUNK(chunk) ((t_malloc_chunk *)((void *)chunk + CHUNKSIZE(chunk)))
# define PREVCHUNK(chunk) ((t_malloc_chunk *)((void *)chunk - chunk->prev_size))

typedef enum s_bitflag
{
	PREV_IN_USE = 1 << 0,	// 1
	ALLOCED = 1 << 1,		// 2
	MAPPED = 1 << 2,		// 4
	ALL = 1 << 3			// 8
}			e_bitflag;

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
	struct s_malloc_chunk	*fd;
	struct s_malloc_chunk	*bk;
}			t_malloc_chunk;

typedef struct s_malloc_footer
{
	size_t			prev_size;
	size_t			size;
	t_malloc_chunk	*fd;
}			t_malloc_footer;

typedef struct s_mmap_chunk
{
	struct s_mmap_chunk	*fd;
	size_t				size;
}				t_mmap_chunk;

typedef struct s_magazine
{
	e_size			type;
	size_t			size; /* region allocation size */
	t_malloc_chunk	*regions;
	t_malloc_chunk	*top;
	t_malloc_chunk	*freelist[64];
}				t_magazine;

typedef	struct s_malloc
{
	bool			is_initialized;
	t_magazine		tiny_magazine;
	t_magazine		small_magazine;
	t_mmap_chunk	*large_allocations;
# ifdef BONUS
	unsigned		flags[TOTAL];
# endif
}			t_malloc;

extern t_malloc	g_malloc;

void	*allocate(size_t size);

void	*malloc_(size_t size);
void	free_(void *ptr);

#endif /* MALLOC_INTERNAL_H */
