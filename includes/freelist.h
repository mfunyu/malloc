#ifndef FREELIST_H
# define FREELIST_H

struct t_malloc_chunk;

void	freelist_add(t_malloc_chunk *freelist[64], t_malloc_chunk *add);
void	freelist_pop(t_malloc_chunk *freelist[64], t_malloc_chunk *pop);
void	*freelist_takeout(t_magazine *magazine, size_t size);

#endif /* FREELIST_H */
