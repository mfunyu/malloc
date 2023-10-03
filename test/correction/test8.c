#include <stdio.h>
#include <string.h>
#include "malloc.h"
#include "ft_printf.h"


void	malloc_alot()
{
	void	*ptr[104];
	for (int i = 0; i < 100; i++)
		ptr[i] = malloc(TINY_MAX);
}

void	main()
{
	ft_printf("=== test8 jsut size for space left malloc ===\n");
	ft_printf("alignment: 8, page_size max\n");

	malloc_alot();
	malloc(600);
	malloc(124);
	malloc(24);
}
