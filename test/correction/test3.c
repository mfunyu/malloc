#include "malloc.h"
#include "ft_printf.h"
#include <string.h>
# include <stdint.h>

# define SIZE 12530990000
# define COUNT (SIZE_MAX / SIZE)

int	main()
{
	void	*ptr;
	size_t	i;

	ft_printf("\n===== test3 Large Test: Nmalloc =====\n");

	i = 0;
	for (; i < COUNT; i++)
	{
		ptr = malloc(SIZE);
		if (!ptr)
			break;
	}
	ft_printf("malloc %zu times, total over %zu bytes\n", i, i * SIZE);
}
