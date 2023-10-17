#include "ft_printf.h"

int		main()
{
	char	*ptr[1000];

	ft_printf("===== test11 time overhead =====\n");
	for (int i = 0; i < 1000; i++) {
		for (int i = 0; i < 1000; i++) {
			int size = rand() & 1023;
			if (size > 1008)
				size = 1008;
			ptr[i] = malloc(size);
		}

		for (int i = 0; i < 500; i++)
			free(ptr[i * 2]);
	}

}
