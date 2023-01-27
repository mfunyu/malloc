#include "libft.h"
#include "ft_printf.h"
#include "malloc.h"

void free(void *ptr)
{
	alloc_debug(__func__, __builtin_return_address(0));
	ft_printf("ptr = %p\n", ptr);
	if (!ptr)
		return ;
	// if (!ALLOC(ptr))
	// {
		// ft_printf("pointer being freeed was not allocated.\n");
		// return;
	// }
	// PUT(HEADER(ptr), PACK(SIZE(ptr), 0));
	// PUT(FOOTER(ptr), PACK(SIZE(ptr), 0));
}
