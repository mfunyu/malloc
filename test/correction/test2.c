#include "malloc.h"
#include "ft_printf.h"
#include <string.h>

void	malloc_write_free(size_t size)
{
	char	*ptr;

	ptr = malloc(size);
	memset(ptr, 'B', size > 100000 ? 100000 : size);
	ft_printf("ptr: %.100s, (%zu)\n", ptr, size);
}

int	main()
{
	ft_printf("\n===== test2 Basic Test2: malloc -> write -> read =====\n");

	malloc_write_free(1);
	malloc_write_free(421);
	malloc_write_free(3421);
	malloc_write_free(100000000);
}
