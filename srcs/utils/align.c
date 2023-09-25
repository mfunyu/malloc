#include "malloc.h"

size_t	align(size_t size, size_t align)
{
	return ((size + (align - 1)) & ~(align - 1));
}

size_t	align_size(size_t size)
{
	return align(size, BYTE);
}

size_t align_chunk_size(size_t size)
{
	if (size < MINSIZE)
		size = MINSIZE;
	size += HEADER_SIZE;
	return align(size, MALLOC_ALIGNMENT);
}
