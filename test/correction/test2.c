#include "malloc.h"
#include "ft_printf.h"

int	main()
{
	void	*ptr;
	ft_printf("=== test2 ===\n");
	ptr = malloc(421);
	memset(ptr, 'a', 42);
	free(ptr);
//	show_alloc_mem();
}
