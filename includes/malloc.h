#ifndef MALLOC_H
# define MALLOC_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

# ifdef BONUS
#  include "bonus.h"
# endif

# define APPROX_PAGE_SIZE 4096
# define MALLOC_ABSOLUTE_SIZE_MAX (SIZE_MAX - (2 * APPROX_PAGE_SIZE))
# define TINY_MAX 1008
# define SMALL_MAX 127 * 8192

# define MALLOC_ALIGNMENT 16
# define MIN_CHUNKSIZE (MALLOC_ALIGNMENT + HEADERSIZE)

# define HEADERSIZE 8
# define FOOTERSIZE sizeof(t_malloc_footer *)
# define LARGE_HEADERSIZE sizeof(t_mmap_chunk *)


typedef enum s_bitflag
{
	PREV_IN_USE = 1 << 0,	// 1
	ALLOCED = 1 << 1,		// 2
	MAPPED = 1 << 2,		// 4
}			e_bitflag;

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

typedef struct s_magazine
{
	size_t			size;
	t_malloc_chunk	*regions;
	t_malloc_chunk	*freelist;
}				t_magazine;

typedef struct s_mmap_chunk
{
	struct s_mmap_chunk	*fd;
	size_t				size;
}				t_mmap_chunk;

typedef	struct s_malloc
{
	bool			is_initialized;
	t_magazine		tiny_magazine;
	t_magazine		small_magazine;
	t_mmap_chunk	*large_allocations;
}			t_malloc;

extern t_malloc	g_malloc;

#endif /* MALLOC_H */
