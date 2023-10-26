#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

int	realloc_same_size()
{
	void	*ptr;
	void	*new_ptr;
	char	*expl = "realloc same as initial size";

	ptr = malloc(12);
	new_ptr = realloc(ptr, 12);

	printf("« %s »\n", expl);
	printf("ptr: %p;\n", ptr);
	printf("new: %p;\n", new_ptr);
	printf("result: ptr %s new_ptr\n", ptr == new_ptr ? "==" : "!=");
	printf("\n");
}

int	realloc_small_size()
{
	void	*ptr;
	void	*new_ptr;
	char	*expl = "realloc smaller than initial size";

	ptr = malloc(42);
	new_ptr = realloc(ptr, 10);

	printf("« %s »\n", expl);
	printf("ptr: %p;\n", ptr);
	printf("new: %p;\n", new_ptr);
	printf("result: ptr %s new_ptr\n", ptr == new_ptr ? "==" : "!=");
	printf("\n");
}


int	realloc_zero_size()
{
	void	*ptr;
	void	*new_ptr;
	char	*expl = "realloc smaller than initial size";

	ptr = malloc(42);
	new_ptr = realloc(ptr, 0);

	printf("« %s »\n", expl);
	printf("ptr: %p;\n", ptr);
	printf("new: %p;\n", new_ptr);
	printf("result: ptr %s new_ptr\n", ptr == new_ptr ? "==" : "!=");
	printf("\n");
}

int	realloc_null_zero()
{
	void	*new_ptr;
	char	*expl = "realloc null size zero";

	new_ptr = realloc(NULL, 0);

	printf("« %s »\n", expl);
	printf("new: %p;\n", new_ptr);
	printf("\n");
}

int	realloc_error()
{
	void	*ptr = realloc(NULL, SIZE_MAX);
	char	*expl = "realloc error";

	printf("« %s »\n", expl);
	if (!ptr)
	{
		printf("ERRNO: %d\n", errno);
		printf("%s\n", strerror(errno));
	}
	printf("\n");
}

int	main()
{
	realloc_same_size();
	realloc_small_size();
	realloc_zero_size();
	realloc_null_zero();
	realloc_error();
}
