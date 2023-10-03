#include <stdio.h>
#include <string.h>
#include "malloc.h"
#include "ft_printf.h"


void	malloc_alot()
{
	void	*ptr[104];
	for (int i = 0; i < 104; i++)
		ptr[i] = malloc(1000);
	ptr[2] = malloc(570);
}

void	main()
{
	ft_printf("=== test6 jsut size for space left malloc ===\n");
	ft_printf("alignment: 16, page_size max\n");

	malloc_alot();
	malloc(32);
	malloc(32);
	malloc(32);
	malloc(32);
	malloc(24);
}
