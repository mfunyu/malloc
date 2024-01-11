#ifndef MALLOC_H
#define MALLOC_H

# include <stdlib.h>

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

void	show_alloc_mem();

# ifdef BONUS
void	show_alloc_mem_ex();
# endif

#endif /* MALLOC_H */
