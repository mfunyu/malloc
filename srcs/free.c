#include <malloc/malloc.h>
#include "libft.h"
#include "ft_printf.h"
#include "malloc.h"

void	free(void *ptr)
{
	malloc_zone_t	*zone;

	alloc_debug(__func__, __builtin_return_address(0));
	ft_printf("ptr = %p\n", ptr);
	if (!ptr)
		return ;

	zone = malloc_zone_from_ptr(ptr);
	if (zone)
		malloc_zone_free(zone, ptr);
	else if (ALLOC(ptr))
	{
		PUT(HEADER(ptr), PACK(SIZE(ptr), 0));
		PUT(FOOTER(ptr), PACK(SIZE(ptr), 0));
	}
	else
		ft_printf("pointer being freeed was not allocated.\n");
}
