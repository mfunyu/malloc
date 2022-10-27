#include "libft.h"
#include "ft_printf.h"

void *realloc(void *ptr, size_t size)
{
	ft_printf("realloc: ptr = %p, size = %d\n", ptr, size);
	return (NULL);
}
