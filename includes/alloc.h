#ifndef ALLOC_H
# define ALLOC_H

# include <stddef.h>

void	*allocate_chunk(size_t size);

void	*map_pages_by_size(size_t map_size);

void	split_chunk(t_heap_chunk *chunk, size_t chunk_size);

#endif /* ALLOC_H */
