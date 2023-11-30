#include "malloc.h"
#include "utils.h"

size_t	align_malloc(size_t size, e_size type)
{
	int		page_size;
	size_t	aligned_size;

	if (type == TINY)
		aligned_size = ALIGN(size + CHUNK_OVERHEAD, TINY_QUANTUM);
	else if (type == SMALL)
		aligned_size = ALIGN(size + CHUNK_OVERHEAD, SMALL_QUANTUM);
	else if (type == LARGE)
	{
		page_size = get_page_size();
		if (page_size == -1)
			return (0);
		aligned_size = ALIGN(size + LARGE_HEADERSIZE, page_size);
	}

	if (aligned_size < MIN_CHUNKSIZE)
		aligned_size = MIN_CHUNKSIZE;
	return (aligned_size);
}
