#include <stdio.h>
#include "malloc.h"
#include "ft_printf.h"

int main()
{
	char *ptr = malloc(21);
	ft_printf("malloced: ptr = %p\n", ptr);
	ptr = realloc(ptr, 42);
	free(ptr);
}
