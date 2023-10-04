#include "malloc.h"
#include "utils.h"

size_t	align(size_t size, size_t align)
{
	return ((size + (align - 1)) & ~(align - 1));
}

size_t	align_size(size_t size)
{
	return align(size, BYTE);
}

size_t	align_chunk_size(size_t size, e_size type)
{
	size_t	aligned_size;
	size_t	total_size;
	int		page_size;

	if (type == TINY || type == SMALL)
	{
		aligned_size = align(size, MALLOC_ALIGNMENT);
		total_size = aligned_size + HEADER_SIZE;
		if (total_size < MINSIZE)
			total_size = MINSIZE;
		return (total_size);
	}
	else if (type == LARGE)
	{
		page_size = get_page_size();
		if (page_size == -1)
			return (0);
		total_size = size + sizeof(t_mmap_chunk);
		aligned_size = align(total_size, page_size);
		return (aligned_size);
	}
	return (0);
}
