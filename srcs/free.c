#include "libft.h"
#include "ft_printf.h"
#include "malloc.h"

void free(void *ptr)
{
	ft_printf("free: ptr = %p\n", ptr);
	if (!ptr)
		return;
	PUT(HEADER(ptr), PACK(SIZE(ptr), 0));
	PUT(FOOTER(ptr), PACK(SIZE(ptr), 0));
}
