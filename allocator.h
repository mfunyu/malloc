#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>

void	*allocate_pages(size_t size, unsigned char align);

#endif /* ALLOCATOR_H */
