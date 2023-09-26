#ifndef ALLOC_H
# define ALLOC_H

# include <stddef.h>

void	*allocate_chunk(size_t size);

void	*map_pages_by_size(size_t map_size);

#endif /* ALLOC_H */
