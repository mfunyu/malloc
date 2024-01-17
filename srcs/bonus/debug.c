#define _GNU_SOURCE
#include <dlfcn.h>
#include "malloc_internal.h"
#include "ft_printf.h"

void	alloc_debug(const char *func_name, void *ret_addr)
{
	Dl_info	info;

	dladdr(ret_addr, &info);
	if (info.dli_sname)
		ft_printf("[%7s] by %-12s : ", func_name, info.dli_sname);
	else
		ft_printf("[%7s] %15s : ", func_name, "");
}

void	malloc_debug(void *ret_addr, size_t size)
{
	if (!g_malloc.flags[DEBUG])
		return ;
	alloc_debug("malloc", ret_addr);
	ft_printf("malloc(%zu)", size);
}

void	free_debug(void *ret_addr, void *ptr)
{
	if (!g_malloc.flags[DEBUG])
		return ;
	alloc_debug("free", ret_addr);
	ft_printf("free(%p)", ptr);
	ft_printf("\n");
}

void	realloc_debug(void *ret_addr, void *ptr, size_t size)
{
	if (!g_malloc.flags[DEBUG])
		return ;
	alloc_debug("realloc", ret_addr);
	ft_printf("realloc(%p, %zu)", ptr, size);
}

void	debug_result(void *ptr)
{
	if (!g_malloc.flags[DEBUG])
		return ;
	ft_printf(" = %p\n", ptr);
}