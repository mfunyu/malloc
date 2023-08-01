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

int main()
{
	/* single free check
	 */
	{
		char*	ptr1;
		ptr1 = set_data(21, '*');
		free(ptr1);
    	set_data(41, 'b');
	}	
	/* basic check
	{
		set_data(21, 'b');
		set_data(45, 'B');
		set_data(12, '*');
	}
	 */
	/* double free check
	{
		char*	ptr1;
		ptr1 = set_data(21, '*');
		free(ptr1);
		char*	ptr2 = set_data(42, '-');
		free(ptr2);
		char*	ptr3 = set_data(21, '+');
		free(ptr3);
	}	
	 */
}
