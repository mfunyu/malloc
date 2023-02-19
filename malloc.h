#ifndef MALLOC_H
# define MALLOC_H

# include <stdlib.h>

# define WORD 4
# define DWORD 8

# define TINY_QUONTAM (1 << 4)
# define TINY_ALIGN DWORD
# define NUM_TINY_BLOCKS 100

# define NO_ALIGNMENT 0

# define SMALL_THRESHOLD (TINY_QUONTAM * (64 - 1))
# define LARGE_THRESHOLD (127 * 1024)
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

typedef struct s_region
{
	void	*head;
	void	*tail;
	size_t	size;
}	t_region;

typedef struct s_zone
{
	/* tiny */
	t_region	tiny_region;

	/* small */
	t_region	small_region;

	/* large */
	t_region	large_region;
}	t_zone;

# define DEBUG 1

# ifdef DEBUG

void	alloc_debug(const char *func_name, void *ret_addr);
# endif

#endif /* MALLOC_H */
