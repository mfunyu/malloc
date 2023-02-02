#ifndef MALLOC_H
# define MALLOC_H

# include <stdlib.h>

# define WORD 4
# define DWORD 8

# define TINY_MAX 0xFF
# define SMALL_MAX 0xFFF

# define PUT(ptr, value) *ptr = value

# define PACK(size, alloc) (size | alloc)
# define SIZE(ptr) (*(unsigned int *)(ptr - WORD) & ~(DWORD - 1))
# define ALLOC(ptr) (*(unsigned int *)(ptr - WORD) & (DWORD - 1))

# define HEADER(ptr) (unsigned int *)(ptr - WORD)
# define FOOTER(ptr) (unsigned int *)(ptr + SIZE(ptr))
# define NEXTPTR(ptr) ptr + SIZE(ptr) + DWORD

# define THICK "\033[1m"
# define CYAN "\033[1;36m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define RESET "\033[m"


char	*g_tiny_head;
char	*g_tiny_max;
char	*g_small_head;
char	*g_small_max;
char	*g_large_head;

# define DEBUG 1

# ifdef DEBUG

void	alloc_debug(const char *func_name, void *ret_addr);
# endif

#endif /* MALLOC_H */
