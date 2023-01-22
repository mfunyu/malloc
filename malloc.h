#ifndef MALLOC_H
# define MALLOC_H

# include <stdlib.h>

# define WORD 4
# define DWORD 8

# define PUT(ptr, value) *ptr = value

# define PACK(size, alloc) (size | alloc)
# define SIZE(ptr) (*(unsigned int *)(ptr - WORD) & ~0x7)
# define ALLOC(ptr) (*(unsigned int *)(ptr - WORD) & 0x1)

# define HEADER(ptr) (unsigned int *)(ptr - WORD)
# define FOOTER(ptr) (unsigned int *)(ptr + SIZE(ptr))
# define NEXTPTR(ptr) ptr + SIZE(ptr) + DWORD

# define THICK "\033[1m"
# define CYAN "\033[1;36m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define RESET "\033[m"

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);

#endif /* MALLOC_H */
