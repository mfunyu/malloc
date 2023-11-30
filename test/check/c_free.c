#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

static void	print_expl(char *expl)
{
	printf("\n=====« %s »=====\n", expl);
}


int	free_dne_align()
{
	void	*ptr = malloc(12);

	print_expl("free next unused address");
	free(ptr + 32);
}

int	free_dne()
{
	void	*ptr = malloc(12);

	print_expl("free address that does not exist");
	free(ptr + 1);
}

int	free_double_free2()
{
	void	*ptr = malloc(12);
	void	*ptr2 = malloc(12);

	print_expl("free causing double free after another free");
	free(ptr);
	free(ptr2);
	void	*ptr3 = malloc(12);
	free(ptr2);
	free(ptr);
}

int	free_double_free()
{
	void	*ptr = malloc(12);

	print_expl("free causing double free");
	free(ptr);
	free(ptr);
}

int	free_info()
{
	void	*ptr = malloc(12);

	print_expl("free check info");
	free(ptr);
	printf("%zu %p %p %zu", *(size_t *)ptr, *(void **)(ptr + 8), *(void **)(ptr + 16), *(size_t *)(ptr + 24));
}

int	main()
{
	free_info();
	// free_double_free2();
	// free_double_free();
	free_dne_align();
	free_dne();
}
