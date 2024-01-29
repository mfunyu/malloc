#include "ft_printf.h"
#include "malloc.h"
#include <string.h>


void	*malloc_write(int i)
{
	void	*ptr;

	ptr = malloc(i);
	memset(ptr, 1, i);
	return (ptr);
}

int	main()
{
	void	*ptr1;
	void	*ptr2;

	ft_printf("\n===== test7 Freelists Test: mallocx2 -> free =====\n");
	
	for (int i = 1025; i <= 127 * 8192; i += 512)
	{
		ptr1 = malloc_write(i);
		ptr2 = malloc_write(i);
		free(ptr1);
	}
}