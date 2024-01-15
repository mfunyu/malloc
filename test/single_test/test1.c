#include "malloc.h"
#include "ft_printf.h"

void	malloc_free_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	ft_printf("before: %p\n", ptr);

	free(ptr);

	ptr = malloc(size);
	ft_printf("after : %p\n", ptr);
	ft_printf("\n");
}

int	main()
{
	size_t	size;

	ft_printf("\n===== test1 Basic Test1: malloc -> free -> malloc =====\n");

	malloc_free_malloc(212);
	malloc_free_malloc(4242);
	malloc_free_malloc(121212);
}
