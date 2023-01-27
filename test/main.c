#include <stdio.h>
#include "malloc.h"
#include "ft_printf.h"

char *set_data(size_t size, int chr)
{
	char *ptr = malloc(size + 1);
	ft_memset(ptr, chr, size);
	ptr[size] = '\0';

	return ptr;
}

void put_data(char *ptr)
{
	size_t size = ft_strlen(ptr);
	ft_printf("[malloced] ptr = %p  size = %d\n	\"%s\"\n", ptr, size, ptr);
}

int main()
{
	char*	ptr1 = set_data(21, '*');
	char*	ptr2 = set_data(22, '-');
	char*	ptr3 = set_data(21, '+');

	put_data(ptr1);
	put_data(ptr2);
	put_data(ptr3);

	// ptr = realloc(ptr, 42);
	// free(ptr);
}
