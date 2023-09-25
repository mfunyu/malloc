#include <unistd.h>
#include "malloc.h"
#include "libft.h"
#include "ft_printf.h"

static int	_get_page_size()
{
	static int	page_size;

	if (page_size)
		return page_size;
	page_size = getpagesize();
	if (page_size == -1) {
		ft_putendl_fd("Error: getpagesize", STDERR_FILENO);
		return (-1);
	}
	return page_size;
}

size_t	calc_map_size(size_t max_block_size)
{
	size_t		map_size;
	static size_t	page_size;

	if (!page_size) {
		page_size = _get_page_size();
        if (!page_size)
            return (-1);
    }
	map_size = page_size * ((max_block_size + MALLOC_ALIGNMENT) * MIN_BLOCKS / page_size);
	ft_printf("mapsize: %d\n", map_size);
	return (map_size);
}