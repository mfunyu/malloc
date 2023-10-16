#include "ft_printf.h"

int		main()
{
	char	*ptr[100];

	ft_printf("===== test11 time overhead =====\n");
	for (int i = 0; i < 3; i++) {
		for (int i = 0; i < 100; i++) {
			int size = rand() & 1023;
			//ft_printf("%d\n", size);
			ptr[i] = malloc(rand() & 1023);
		}
		/*
		for (int i = 0; i < 50; i++)
			free(ptr[i * 2]);
		for (int i = 0; i < 100; i++)
			ptr[i] = malloc(1100 + i);
		for (int i = 0; i < 100; i++)
			free(ptr[i]); */
	}
}
