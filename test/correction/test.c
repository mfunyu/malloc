#include "malloc.h"
#include "ft_printf.h"

int	main()
{
	void	*ptr[100];
	int		i;

	ft_printf("\n===== test0 Basic Test: malloc & free =====\n");

	/* malloc */
	i = 0;
	ptr[i++] = malloc(0);
	ptr[i++] = malloc(1);
	ptr[i++] = malloc(42);
	ptr[i++] = malloc(100);
	ptr[i++] = malloc(1000);
	ptr[i++] = malloc(1024 - 1);
	ptr[i++] = malloc(1024); /* TINY_MAX */
	ptr[i++] = malloc(1024 + 1);
	ptr[i++] = malloc(200000);
	ptr[i++] = malloc(1040384 - 1);
	ptr[i++] = malloc(1040384); /* SMALL_MAX */
	ptr[i++] = malloc(1040384 + 1);
	ptr[i++] = malloc(500000000);
	ptr[i++] = malloc(3000000000);
	ptr[i++] = malloc(10000000000);
	ptr[i++] = malloc(12500000000);
	ptr[i++] = malloc(12530990000); /* fail after this value */
	ptr[i++] = malloc(70000000000);
	ptr[i++] = malloc(9223372036854775807);
	ptr[i++] = malloc(18446744073709543423);

	ft_printf("\n - after malloc -\n");
	show_alloc_mem();

	/* free */
	for (; i > 0; i--)
		free(ptr[i - 1]);

	ft_printf("\n - after free -\n");
	show_alloc_mem();
}
