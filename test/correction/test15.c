#include "ft_printf.h"
#include "debug.h"
#include "malloc.h"
#include "bonus.h"
#include <stdlib.h>

int		size_generator()
{
	int size = rand() & 1023;
	if (size > TINY_MAX)
		size = TINY_MAX;
	return size;
}

int		main(int ac, char **av)
{
	char	*ptr[1000];
	size_t	size[1000];
	int		loop_times = 100;
	if (ac > 1)
		loop_times = atoi(av[1]);

	ft_printf("\n===== test15: many realloc compare address =====\n");

	for (int j = 0; j < loop_times; j++)
	{
		for (int i = 0; i < 100; i++)
		{
			size[i] = size_generator();
			ptr[i] = malloc(size[i]);
			if ((uintptr_t)ptr[i] & (TINY_QUANTUM - 1))
			{
				SP("malloc", ptr[i]);
				SD("indexj", j);
				SD("indexi", i);
				SD("size", size[i]);
			}
		}
		for (int i = 0; i < 100; i++)
		{
			//SP("realloc", ptr[i]);
			ptr[i] = realloc(ptr[i], size_generator());
		}
	}
}
