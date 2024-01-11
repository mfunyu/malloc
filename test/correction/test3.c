#include "malloc.h"
#include "ft_printf.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

# define STORE 1000000
# define TINY_MAX 1024

size_t	i;

void	print_result(void)
{
	ft_printf("\nmalloc %zu times, total around %zu bytes\n", i, i * TINY_MAX);
}

void	sig_handler(int signo)
{
	if (signo == SIGINT)
	{
		print_result();
		exit(0);
	}
}

int	main()
{
	void	*ptr[STORE];
	int		size;

	signal(SIGINT, sig_handler);
	ft_printf("\n===== test3 Tiny Test: Nmalloc =====\n");

	for (int j = 0; i < SIZE_MAX; i++)
	{
		size = rand() & (TINY_MAX - 1);
		if (size == 0)
			size = 1;
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
	print_result();
}
