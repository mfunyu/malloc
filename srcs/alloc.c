#include <stddef.h>
#include <stdbool.h>
#include "malloc.h"
#include "alloc_error.h"
#include "allocator.h"

static bool g_alloc_is_initialized = false;
static t_zone *g_zone;
static size_t g_page_size;

static size_t	round_page(size_t size)
{
	return (size + g_page_size - 1) & ~(g_page_size - 1);
}


static void	alloc_initialize(void)
{
	if (g_alloc_is_initialized)
		return ;
	g_alloc_is_initialized = true;
	g_page_size = getpagesize();
	g_zone = allocate_pages(sizeof(t_zone), NO_ALIGNMENT);
	if (!g_zone)
		error_exit();
}

static void	*tiny_alloc_from_new_region(t_zone *zone, size_t size)
{
	void	*aligned_addr;
	void	*first_block;
	size_t	tiny_region_size;

	tiny_region_size = round_page(TINY_QUONTAM * NUM_TINY_BLOCKS);
	aligned_addr = allocate_pages(tiny_region_size, TINY_ALIGN);
	if (!aligned_addr)
		return NULL;

	/* add header */
	PUT(HEADER(aligned_addr), PACK(TINY_ALIGN, 1));
	PUT(FOOTER(aligned_addr), PACK(TINY_ALIGN, 1));
	/* add footer */
	PUT(FOOTER(aligned_addr + tiny_region_size), PACK(0, 1));


}

static void	*tiny_alloc_should_clear(t_zone *zone, size_t size, bool should_clear)
{
	void	*ptr;

	/* find from free-list */

	/* find from end */
	if (zone->tiny_region.size)
	{

	}

	/* find from new region */
	ptr = tiny_alloc_from_new_region(zone, size);

	return ptr;
}

void	*alloc_should_clear(size_t size, bool should_clear)
{
	void			*ptr;
	static t_zone	*zone;

	if (!g_alloc_is_initialized)
		alloc_initialize();

	zone = g_zone;
	if (size <= SMALL_THRESHOLD) {
		ptr = tiny_alloc_should_clear(zone, size, should_clear);
	} else if (size <= LARGE_THRESHOLD) {
		ptr = small_alloc_should_clear(zone, size, should_clear);
	} else {
		ptr = large_alloc();
	}

	return ptr;
}
