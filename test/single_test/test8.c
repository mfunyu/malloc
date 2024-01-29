#include "ft_printf.h"
#include "malloc.h"
#include <errno.h>
#include <string.h>

void	call_realloc(void *ptr, size_t add, size_t size)
{
	void	*ptr2;

	ft_printf("+%d (%p) -> ", add, ptr + add);
	ptr2 = realloc(ptr + add, size);
	if (errno) {
		ft_printf("Error: %s\n", strerror(errno));
		errno = 0;
	}
	else if (ptr2)
		ft_printf("%p\n", ptr2);
}

int	main()
{
	void	*ptr1;

	ft_printf("\n===== test8 Invalid Address Test: realloc =====\n");
	
	ptr1 = malloc(100);

	ft_printf("\n[[realloc] address existing, already freed]\n");
	free(ptr1);
	call_realloc(ptr1, 100, 1000);

	ft_printf("\n[[realloc] address existing, non aligned]\n");
	for (int i = 1; i <= 8; i++)
		call_realloc(ptr1, i, 1000);

	ft_printf("\n[[realloc] address existing, aligned]\n");
	for (int i = 16; i < 100; i += 16)
		call_realloc(ptr1, i, 1000);

	
	ft_printf("\n[[realloc] address existing, non aligned, size=0]\n");
	for (int i = 1; i <= 8; i++)
		call_realloc(ptr1, i, 0);
}