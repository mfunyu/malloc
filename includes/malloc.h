#ifndef MALLOC_H
#define MALLOC_H

# include <stddef.h>

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

void	show_alloc_mem();

#endif /* MALLOC_H */
