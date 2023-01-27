#include "libft.h"
#include "malloc.h"
#include "ft_printf.h"

void	*realloc(void *ptr, size_t size)
{
	alloc_debug(__func__, __builtin_return_address(0));
	ft_printf("ptr = %p, size = %d\n", ptr, size);
	if (!ptr)
		ptr = malloc(size);
	return (ptr);
}
