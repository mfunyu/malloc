#include "malloc.h"
#include "ft_printf.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

# define STORE 1000000
# define TINY_MAX 1024

int	main()
{
	void	*ptr[STORE];
	size_t	i;
	int		size;

	ft_printf("\n===== test3 Tiny Test: Nmalloc =====\n");

	i = 0;
	for (int j = 0; i < SIZE_MAX; i++)
	{
		size = rand() & (TINY_MAX - 1);
		ptr[j] = malloc(size);
		if (!ptr[j])
		{
			ft_printf("Error: %s\n", strerror(errno));
			break ;
		}
		if (j == STORE) /* free from old to new */
		{
			for (int k = 0; k < j; k++)
				free(ptr[k]);
			j = 0;
		}
	}
	ft_printf("malloc %zu times, total around %zu bytes\n", i, i * TINY_MAX);
}
