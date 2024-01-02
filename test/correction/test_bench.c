#include "ft_printf.h"
#include "malloc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define TINY_MAX 1024

int		main(int ac, char **av)
{
	char	*ptr[1000];
	int		loop_times = 1000;
	if (ac > 1)
		loop_times = atoi(av[1]);

	ft_printf("\n===== Benchmark-tiny: time overhead =====\n");
	ft_printf("%d loops of 1000 mallocs and 500 free\n", loop_times);
	ft_printf("-> %d mallocs, %d frees\n", loop_times * 1000, loop_times * 500);

	for (int i = 0; i < loop_times; i++)
	{
		for (int i = 0; i < 1000; i++)
		{
			int size = rand() & (TINY_MAX - 1);
			ptr[i] = malloc(size);
			if (!ptr[i])
			{
				ft_printf("Error: %s\n", strerror(errno));
				exit(1);
			}
		}

		for (int i = 0; i < 500; i++)
			free(ptr[i * 2]);
	}
}
