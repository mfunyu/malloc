#include "malloc.h"
#include "ft_printf.h"
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

# define SIZE 12530990000
# define COUNT (SIZE_MAX / SIZE)
# define STORE 1000000

size_t	i;

void	print_result(void)
{
	ft_printf("\nmalloc %zu times, total over %zu bytes\n", i, i * SIZE);
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

	signal(SIGINT, sig_handler);
	ft_printf("\n===== test5 Large Test: Nmalloc =====\n");

	for (int j = 0; i < COUNT; i++)
	{
		ptr[j] = malloc(SIZE);
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
