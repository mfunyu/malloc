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

int	malloc_error()
{
	void	*ptr = malloc(SIZE_MAX);
	char	*expl = "malloc error";

	print_expl("malloc error");
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
		printf("%p : %d (%d [%p])\n", prev, arr[0], ptr - prev, ptr - prev);
	if (!arr[1])
		return ;
	return malloc_rec(arr + 1, ptr);
}

int malloc_address()
{
	int		arr[20] = {0, 1, 16, 24, 25, 32, 33, 40, 41, 48};

	print_expl("malloc check return address");
	malloc_rec(arr, NULL);
}

int	main()
{
	malloc_zero();
	malloc_error();
	malloc_address();
}
