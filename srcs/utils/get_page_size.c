#include <unistd.h>
#include "ft_printf.h"

int		get_page_size()
{
	int	page_size;

	page_size = getpagesize();
	if (page_size == -1) {
		ft_printf("Error: getpagesize");
		return (-1);
	}
	return page_size;
}
