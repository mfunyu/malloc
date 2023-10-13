#include "malloc.h"
#include "utils.h"

size_t	align(size_t size, size_t align)
{
	return ((size + (align - 1)) & ~(align - 1));
}

size_t	align_malloc(size_t size)
{
	return (align(size, MALLOC_ALIGNMENT));
}

size_t	align_malloc_chunk(size_t size)
{
	size_t	aligned_size;
	size_t	total_size;

	aligned_size = align_malloc(size);
	total_size = aligned_size + CHUNK_HEADERSIZE;
	if (total_size < MIN_CHUNKSIZE)
		total_size = MIN_CHUNKSIZE;
	return (total_size);
}

size_t	align_large(size_t size)
{
	int		page_size;

	page_size = get_page_size();
	if (page_size == -1)
		return (0);
	return(align(size + LARGE_HEADERSIZE, page_size));
}