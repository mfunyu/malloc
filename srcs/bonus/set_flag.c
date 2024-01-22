#include "malloc_internal.h"
#include "ft_printf.h"
#include "bonus.h"
#include <stdlib.h>

void	set_flags_from_environment()
{
	if (getenv("MallocShowHeap"))
		g_malloc.flags[SHOW_HEAP] = 1;
	if (getenv("MallocShowAbbr"))
		g_malloc.flags[SHOW_ABBR] = 1;
	if (getenv("MallocShowFree"))
		g_malloc.flags[SHOW_FREE] = 1;
	if (getenv("MallocDebug"))
		g_malloc.flags[DEBUG] = 1;
	if (getenv("MallocHelp")) {
		ft_printf("environment variables that can be set for debug:\n"
		"- MallocShowHeap - display heap in detail\n"
		"- MallocShowAbbr - display heap\n"
		"- MallocShowFree - display freelist\n"
		"- MallocDebug - display log for each *alloc, free call\n"
		"- MallocHelp - show this help!\n");
	}
}
