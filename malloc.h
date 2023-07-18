#ifndef MALLOC_H
# define MALLOC_H

#include <stdbool.h>

#define APPROX_PAGE_SIZE 4096
#define MALLOC_ABSOLUTE_SIZE_MAX (SSIZE_MAX - (2 * APPROX_PAGE_SIZE))

#define ALIGNMENT 16

#define TINY_MAX 1008
#define SMALL_MAX 127 * 8192
#define MIN_BLOCKS 100

typedef enum s_size
{
	TINY,
	SMALL,
	LARGE
}			e_size;

typedef struct s_region
{
	void	*head;
	void	*tail;
	void	*freelist;
}				t_region;

typedef struct s_malloc
{
	bool		initialized;
	t_region	tiny_region;
	t_region	small_region;
	t_region	large_region;
}			t_malloc;

extern t_malloc	regions;

#endif
