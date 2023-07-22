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

# define NEXTPTR(ptr) (unsigned int **)(ptr + WORD)
# define PREVPTR(ptr) (unsigned int **)(ptr + WORD + BYTE)

typedef enum s_size
{
	TINY,
	SMALL,
	LARGE
}			e_size;

typedef struct s_region
{
	size_t	map_size;
	void	*head;
	void	*tail;
	void	*mapped_till;
	void	*freelist;
}				t_region;

typedef struct s_malloc
{
	bool		initialized;
	t_region	tiny_region;
	t_region	small_region;
	t_region	large_region;
}			t_malloc;

extern t_malloc	g_regions;

void	show_free_list(t_region region);

#endif
