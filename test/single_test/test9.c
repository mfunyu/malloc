#include "ft_printf.h"
#include "malloc.h"
#include <errno.h>
#include <string.h>

void	call_free(void *ptr, size_t add)
{
	void	*ptr2;

	ft_printf("+%d (%p) -> ", add, ptr + add);
	free(ptr + add);
	ft_printf("\n");
}

int	test_free(size_t size)
{
	void	*ptr1;
	ptr1 = malloc(size);

	ft_printf("\n[[free] address existing, already freed]\n");
	free(ptr1);
	call_free(ptr1, 100);

	ft_printf("\n[[free] address existing, non aligned]\n");
	for (int i = 1; i <= 8; i++)
		call_free(ptr1, i);

	ft_printf("\n[[free] address existing, aligned]\n");
	for (int i = 16; i < 100; i += 16)
		call_free(ptr1, i);
}

int	main()
{

	ft_printf("\n===== test8 Invalid Address Test: free =====\n");
	test_free(100);
	test_free(1000000);
}