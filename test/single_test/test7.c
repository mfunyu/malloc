#include "ft_printf.h"
#include "malloc.h"

int	main()
{
	void	*ptr1;
	void	*ptr2;

	ft_printf("\n===== test7 Freelists Test: mallocx2 -> free =====\n");
	for (int i = 0; i <= 1024; i += 16)
	{
		ptr1 = malloc(i);
		ptr2 = malloc(i);
		free(ptr1);
	}
	for (int i = 1025; i < 127 * 8912; i += 512)
	{
		ptr1 = malloc(i);
		ptr2 = malloc(i);
		free(ptr1);
	}
}