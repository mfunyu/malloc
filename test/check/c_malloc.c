#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

static void	print_expl(char *expl)
{
	printf("« %s »\n", expl);
}

int	malloc_zero()
{
	void	*ptr = malloc(0);

	print_expl("malloc size zero");
	printf("ptr: %p;\n", ptr);
	printf("\n");
}

int	malloc_error(size_t size)
{
	void	*ptr = malloc(size);
	char	*expl = "malloc error";

	printf("SIZE : %zu\n", size);
	if (!ptr)
	{
		printf("ERRNO: %d\n", errno);
		printf("%s\n", strerror(errno));
	}
	printf("\n");
}

void	malloc_rec(int *arr, void *prev)
{
	void	*ptr;

	if (arr[1])
		ptr = malloc(arr[1]);
	else
		ptr = malloc(0);
	if (prev)
		printf("%p : %d (%ld [%p])\n", prev, arr[0], ptr - prev, ptr - prev);
	if (!arr[1])
		return ;
	return malloc_rec(arr + 1, ptr);
}

int malloc_address()
{
	int		arr[20] = {0, 1, 16, 24, 25, 32, 33, 40, 41, 48};

	print_expl("malloc check return address");
	malloc_rec(arr, NULL);
	printf("\n");
}

void	malloc_large()
{
	print_expl("malloc large");
	malloc_error(3000000000);
	malloc_error(10000000000);
	malloc_error(12500000000);
	malloc_error(12550000000);
	malloc_error(13000000000);
	malloc_error(13750000000);
	malloc_error(15000000000);
	malloc_error(20000000000);
	malloc_error(50000000000);
	malloc_error(70000000000);
	malloc_error(9223372036854775807);
	malloc_error(SIZE_MAX);
}

int	main()
{
	malloc_zero();
	malloc_address();
	malloc_large();
}
