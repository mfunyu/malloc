#include "malloc.h"
#include "ft_printf.h"

int	main()
{
	ft_printf("=== test ===\n");
	malloc(1);
	malloc(42);
	malloc(1024);
	malloc(200000);
	malloc(500000000);
	show_alloc_mem();
}
