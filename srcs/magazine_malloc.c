#include <bool.h>
#include "malloc.h"

static t_zone	*g_zone;
static int		g_page_size;
static bool		g_malloc_is_initialized = false;

static size_t	round_page(size_t size)
{
	int		page_size;

	page_size = g_page_size;
	if (!page_size)
		page_size = getpagesize();
	return ((size + page_size - 1) & ~(page_size - 1));
}

static void	*allocate_pages(size_t size)
{
	size_t	allocation_size;
	void	*addr;

	allocation_size = round_page(size);
	if (allocation_size < size)
		error_exit();

	addr = (char *)mmap(0, allocation_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (addr == MAP_FAILED) {
		error_exit();
	}
	return addr;
}

static void	malloc_initialize(void)
{
	if (g_malloc_is_initialized)
		return ;
	g_malloc_is_initialized = true;
	g_page_size = getpagesize();
	//allocate zone
}

static void tiny_allocate_from_new_region(t_magazine *mag)
{
	ptr = allocate_pages(TINY_QUONTAM * NUM_TINY_BLOCKS);
	mag->regions = ptr;

}

static void	*tiny_malloc_should_clear(t_magazine *mag, size_t size, bool cleared_requested)
{
	if (mag->)
	ptr = tiny_allocate_from_new_region(mag);
	return ptr;
}

static void	small_malloc_should_clear(t_magazine *mag, size_t size, bool cleared_requested)
{

}

static void	large_malloc_should_clear(t_magazine *mag, size_t size, bool cleared_requested)
{

}


void	malloc_should_clear(size_t size, bool cleared_requested)
{
	t_zone	*zone;

	zone = g_zone;
	if (size <= SMALL_THRESHOLD)
	{
		tiny_malloc_should_clear(zone->mag_tiny, size, cleared_requested);
	}
	else if (size <= LARGE_THRESHOLD)
	{
		small_malloc_should_clear(zone->mag_small, size, cleared_requested);
	}
	else
	{
		large_malloc_should_clear(zone->mag_large, size, cleared_requested);
	}
}
