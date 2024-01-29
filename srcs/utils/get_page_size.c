#include <unistd.h>
#include "utils.h"

int		get_page_size()
{
	static int	page_size;

	if (page_size > 0)
		return (page_size);
	page_size = getpagesize();
	if (page_size == -1)
		return (print_error_ret_value("getpagesize failed", -1));
	return (page_size);
}
