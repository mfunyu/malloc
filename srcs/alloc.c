#include <sys/mman.h>
#include "ft_printf.h"

void	*alloc_pages_by_size(size_t map_size, void *start)
{
	void	*ptr;

	ptr = mmap(start, map_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (ptr == MAP_FAILED) {
		ft_printf("Error: mmap failed - size %d\n", map_size);
        return (NULL);
	}
	return (ptr);
}
