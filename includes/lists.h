#ifndef LISTS_H
#define LISTS_H

/* lst_malloc_chunk.c */
void	lst_malloc_chunk_add_front(t_malloc_chunk **lst, t_malloc_chunk *new_chunk);
void	lst_malloc_chunk_insert(t_malloc_chunk *prev, t_malloc_chunk *insert);
void	lst_malloc_chunk_replace(t_malloc_chunk **lst,
			t_malloc_chunk *old, t_malloc_chunk *new_chunk);
void	lst_malloc_chunk_pop(t_malloc_chunk **lst, t_malloc_chunk *pop);
void	lst_malloc_chunk_sort_add(t_malloc_chunk **head, t_malloc_chunk *chunk);

/* lst_mmap_chunk.c */
void	lst_mmap_chunk_add(t_mmap_chunk **head, t_mmap_chunk *chunk);
void	lst_mmap_chunk_pop(t_mmap_chunk **head, t_mmap_chunk *chunk);

#endif /* LISTS_H */
