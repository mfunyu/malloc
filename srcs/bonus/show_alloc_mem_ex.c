#include "malloc_internal.h"
#include "print.h"
#include "bonus.h"

void show_alloc_mem_ex(void)__attribute__((destructor));

void	show_alloc_mem_ex()
{
	if (!g_malloc.flags[SHOW_HEAP] && !g_malloc.flags[SHOW_ABBR])
		return ;
	print_malloc(g_malloc.tiny_magazine, TINY);
	show_freelist(g_malloc.tiny_magazine);
	print_malloc(g_malloc.small_magazine, SMALL);
	show_freelist(g_malloc.small_magazine);
	print_large(g_malloc.large_allocations);
}
