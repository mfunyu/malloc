#include <sys/mman.h>
#include "utils.h"

void	*mmap_by_size(size_t map_size)
{
	void	*ptr;

	ptr = mmap(NULL, map_size, PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_PRIVATE, -1, 0);
	if (ptr == MAP_FAILED)
		return (error_null("mmap failed"));
	return (ptr);
}
