#include "malloc.h"
#include <string.h>
#include <stdio.h>

# define ft_printf printf

int	test(void *ptr, int diff)
{
	ft_printf("<ptr + %d>\n", diff);
	ptr += diff;
	ft_printf("test: %p\n", ptr);
	ft_printf("test: \"%s\"\n", ptr);
	free(ptr);
	ft_printf("\n");
}

int	main()
{
	void	*prev = malloc(48);
	void	*ptr = malloc(48);
	// memset(prev, 'B', 48);
	// memset(ptr, 'a', 42);

	ft_printf("\n===== test16: free wrong place =====\n");
	void	*test1 = ptr - 3;
	void	*test2 = ptr - 8;
	void	*test3 = ptr + 8;
	ft_printf("prev: %p\n", prev);
	ft_printf("ptr: %p\n", ptr);
	test(ptr, 0);
	test(ptr, -24);
	test(ptr, 8);
	test(ptr, -3);
	test(ptr, -8);
}
