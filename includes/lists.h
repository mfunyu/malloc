#ifndef LISTS_H
#define LISTS_H

/* lst_mmap_chunk.c */
void	lst_mmap_chunk_add(t_mmap_chunk **head, t_mmap_chunk *chunk);
void	lst_mmap_chunk_pop(t_mmap_chunk **head, t_mmap_chunk *chunk);

#endif /* LISTS_H */
