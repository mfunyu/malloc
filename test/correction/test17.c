#include "ft_printf.h"
#include "debug.h"
#include "malloc.h"
#include "bonus.h"
#include <stdlib.h>

int		main(int ac, char **av)
{
	char	*ptr[1000];
	size_t	size[1000];
	int		loop_times = 100;
	if (ac > 1)
		loop_times = atoi(av[1]);

	ft_printf("\n===== test17: many SMALL malloc compare address =====\n");
	ft_printf("%d loops of 1000 mallocs and 500 free\n", loop_times);
	ft_printf("-> %d mallocs, %d frees\n", loop_times * 100, loop_times * 50);

	for (int i = 0; i < loop_times; i++)
	{
		for (int i = 0; i < 1000; i++)
		{
			int s = rand() & SMALL_MAX;
			if (s <= TINY_MAX)
				s = TINY_MAX + 1;
			size[i] = s;
			D(s);
			ptr[i] = malloc(s);
		}
		for (int i = 0; i < 500; i++)
		{
			free(ptr[i * 2]);
		}
	}
}
