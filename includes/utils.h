#ifndef UTILS_H
# define UTILS_H

#include <stddef.h>

/* align.c */
size_t	align(size_t size, size_t align);
size_t	align_size(size_t size);
size_t align_chunk_size(size_t size);

/* page_size.c */
int		get_page_size();
size_t	calc_map_size(size_t max_block_size);

#endif /* UTILS_H */