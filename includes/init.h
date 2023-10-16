#ifndef INIT_H
# define INIT_H

# include <stddef.h>

struct t_malloc_chunk;

int				init_malloc();
t_malloc_chunk	*init_region(size_t	size);

#endif /* INIT_H */
