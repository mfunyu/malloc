#include "malloc.h"
#include "print.h"

void show_alloc_mem_ex(void)__attribute__((destructor));

void	show_alloc_mem_ex()
{
	if (!malloc_show_heap && !malloc_show_abbr)
		return ;
	print_malloc(g_malloc.tiny_magazine, TINY);
	show_freelist(g_malloc.tiny_magazine);
	print_malloc(g_malloc.small_magazine, SMALL);
	show_freelist(g_malloc.small_magazine);
	print_large(g_malloc.large_allocations);
}
