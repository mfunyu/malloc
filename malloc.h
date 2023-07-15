#ifndef MALLOC_H
# define MALLOC_H

#include <stdbool.h>

#define ALIGNMENT 8

#define TINY_MAX 1008
#define SMALL_MAX 127 * 8192
#define MIN_BLOCKS 100

typedef struct s_malloc
{
	bool	initialized;
	void	*tiny_zone;
	void	*small_zone;
	void	*large_zone;
}			t_malloc;

extern t_malloc	zones;

#endif
