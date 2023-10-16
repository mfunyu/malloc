#include <stdio.h>
#include <string.h>
#include "malloc.h"
#include "ft_printf.h"

void	main()
{
	ft_printf("=== test10 jsut size for space left malloc ===\n");

	void	*ptr1 = malloc(16);
	void	*ptr2 = malloc(16);
	free(ptr1);
	free(ptr2);
}
