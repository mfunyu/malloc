#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>

struct t_malloc_chunk;

int		get_page_size();

void	*mmap_by_size(size_t map_size);

/* alignment.c */
size_t	align(size_t size, size_t align);
size_t	align_malloc(size_t size);
size_t	align_large(size_t size);

void	lst_mmap_chunk_add(t_mmap_chunk **head, t_mmap_chunk *chunk);

void	*split_chunk(t_malloc_chunk *chunk, size_t new_size);

void	lst_malloc_chunk_replace(t_malloc_chunk **lst,
			t_malloc_chunk *old, t_malloc_chunk *new_chunk);
void	lst_malloc_chunk_pop(t_malloc_chunk **lst, t_malloc_chunk *pop);

#endif /* UTILS_H */
