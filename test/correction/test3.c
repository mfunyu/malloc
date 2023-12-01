#include "malloc.h"
#include "ft_printf.h"
#include <string.h>
# include <stdint.h>

# define SIZE 12530990000
# define COUNT (SIZE_MAX / SIZE)
# define STORE 1000000

int	main()
{
	void	*ptr[STORE];
	size_t	i;

	ft_printf("\n===== test3 Large Test: Nmalloc =====\n");

	i = 0;
	for (int j = 0; i < COUNT; i++)
	{
		ptr[j] = malloc(SIZE);
		if (!ptr[j])
			break;
		if (j == STORE) /* free from old to new */
		{
			for (int k = 0; k < j; k++)
				free(ptr[k]);
			j = 0;
		}
	}
	ft_printf("malloc %zu times, total over %zu bytes\n", i, i * SIZE);
}
