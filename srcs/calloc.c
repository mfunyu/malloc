#include "libft.h"
#include "ft_printf.h"
#include "malloc.h"
# include <errno.h>
/*
void	*calloc(size_t count, size_t size)
{
	void	*ptr;
	static int cnt;

	ptr = ft_calloc(count, size);
	alloc_debug(__func__, __builtin_return_address(0));
	ft_printf("calloc: ptr = %p, size = %d, %d\n", ptr, count * size, cnt++);
	if (ptr == NULL) {
		errno = ENOMEM;
	}
	return ptr;
}
*/
