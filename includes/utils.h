#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>

struct t_malloc_chunk;

int		get_page_size();

void	*mmap_by_size(size_t map_size);

/* alignment.c */
size_t	align(size_t size, size_t align);
size_t	align_malloc(size_t size);
size_t	align_malloc_chunk(size_t size);
size_t	align_large(size_t size);

void	*split_chunk(t_malloc_chunk *chunk, size_t new_size);

/* error.c */
void	*error_null(char *error_msg);
int		error_ret(char *error_msg, int ret);
void	error_msg(char *error_msg);

#endif /* UTILS_H */
