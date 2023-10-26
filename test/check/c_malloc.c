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

int	main()
{
	malloc_zero();
	malloc_error();
}
