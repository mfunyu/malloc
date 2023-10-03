#include <stdio.h>
#include <string.h>
#include "malloc.h"
#include "ft_printf.h"

void	main()
{
	ft_printf("=== test9 realloc extending chunk ===\n");

	void	*ptr = malloc(100);
	void	*new_ptr = realloc(ptr, 600);

	ft_printf("ptr: %p;\n", ptr);
	ft_printf("new: %p;\n", new_ptr);
	ft_printf("result: ptr %s new_ptr\n", ptr == new_ptr ? "==" : "!=");
}
