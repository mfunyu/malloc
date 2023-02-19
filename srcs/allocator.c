#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>
#include "alloc_error.h"

static void	*align_address(void *addr, unsigned char align,
						size_t allocation_size, size_t size)
{
	void	*aligned_addr;
	size_t	delta;

	aligned_addr = ((uintptr_t)addr + 1 << align - 1) & ~(1 << align - 1);
	if (aligned_addr != addr) {
		delta = addr - aligned_addr;
		if (mumap(addr, delta) == -1)
			error_print_exit("*** munmap unaligned header failed with ");
		allocation_size -= delta;
	}
	if (allocation_size > size) {
		if (mumap(addr + size, allocation_size - size) == -1)
			error_print_exit("*** munmap unaligned footer failed with ");
	}
	return aligned_addr;
}

void	*allocate_pages(size_t size, unsigned char align)
{
	size_t	allocation_size;
	void	*addr;

	if (align)
		allocation_size += 1 << align;
	if (allocation_size < size)
		return NULL;

	addr = (char *)mmap(0, allocation_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (addr == MAP_FAILED) {
		return NULL;
	}

	if (align)
		addr = align_address(addr, align, allocation_size, size);

	return addr;
}
