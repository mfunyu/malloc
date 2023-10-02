#include <stdio.h>
#include <stdlib.h>
#include "ft_printf.h"
#include "malloc.h"

int main()
{
	ft_printf("%p\n", malloc(MALLOC_ABSOLUTE_SIZE_MAX));
	//printf("%lu\n", SSIZE_MAX);
}
