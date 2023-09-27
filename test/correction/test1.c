#include "malloc.h"
#include "ft_printf.h"

int	main()
{
	malloc(1);
	malloc(42);
	malloc(1024);
	malloc(200000);
	malloc(500000000);
	malloc(500000000);
	malloc(900000000);
	for (int i = 0; i < 10; i++) malloc(1120000000);
	show_alloc_mem();
}
