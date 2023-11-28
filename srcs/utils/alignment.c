#include "malloc.h"
#include "utils.h"

size_t	align_malloc(size_t size)
{
	return (ALIGN(size, MALLOC_ALIGNMENT));
}

size_t	align_malloc2(size_t size, e_size type)
{
	if (type == TINY)
		return (ALIGN(size, TINY_QUANTUM));
	if (type == SMALL)
		return (ALIGN(size, SMALL_QUANTUM));
	return (0);
}

size_t	align_malloc_chunk(size_t size, e_size type)
{
	size_t	aligned_size;

	aligned_size = align_malloc2(size + CHUNK_OVERHEAD, type);
	if (aligned_size < MIN_CHUNKSIZE)
		aligned_size = MIN_CHUNKSIZE;
	return (aligned_size);
}

size_t	align_large(size_t size)
{
	int		page_size;

	page_size = get_page_size();
	if (page_size == -1)
		return (0);
	return(ALIGN(size + LARGE_HEADERSIZE, page_size));
}
