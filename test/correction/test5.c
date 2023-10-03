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

int main()
{
	ft_printf("=== test5 use up malloc ===\n");

	malloc_alot();
	malloc(150);
	malloc(1);
	malloc(1);
}

