#include <stdio.h>
#include "malloc.h"
#include "ft_printf.h"

char *set_data(size_t size, int chr)
{
	char *ptr = malloc(size + 1);
	ft_memset(ptr, chr, size);
	ptr[size] = '\0';

	ft_printf("%p: %c\n", ptr, chr);
	return ptr;
}

void	check_free()
{
	char*	ptr1;
	(void)ptr1;
	ptr1 = set_data(21, '*');
	ptr1 = set_data(43, 'b');
	ptr1 = set_data(4, 'c');
	free(ptr1);
	ptr1 = set_data(43, '!');
	free(ptr1);
}

void	check_small_alloc()
{
	set_data(1, '1');
	set_data(2, '2');
	set_data(3, '3');
	set_data(4, '4');
}

void	malloc_alot()
{
	void	*ptr[104];
	for (int i = 0; i < 104; i++)
		ptr[i] = malloc(1000);
	ptr[2] = malloc(570);
	free(ptr[0]);
}

void	malloc_large()
{
	void *ptr = malloc(SMALL_MAX);
	ft_printf("%p\n", ptr);	
	free(ptr);
}

int main()
{
	//malloc_alot();
	malloc_large();
	//check_small_alloc();
	//check_free();
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
