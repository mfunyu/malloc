# define __USE_GNU
#include <dlfcn.h>
#include "ft_printf.h"

void	alloc_debug(const char *func_name, void *ret_addr)
{
	Dl_info	info;

	dladdr(ret_addr, &info);
	ft_printf("[%s] %s calls ", func_name, info.dli_sname);
}
