#include "ft_printf.h"
#include <stdlib.h>

unsigned	malloc_show_heap = 0;
unsigned	malloc_show_abbr = 0;

void	set_flags_from_environment()
{
	if (getenv("MallocShowHeap"))
		malloc_show_heap = 1;
	if (getenv("MallocShowAbbr"))
		malloc_show_abbr = 1;
	if (getenv("MallocHelp")) {
		ft_printf("environment variables that can be set for debug:\n"
		"- MallocShowHeap - dispay heap\n"
		"- MallocHelp - show this help!\n");
	}
}
