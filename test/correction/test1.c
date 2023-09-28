#include "malloc.h"
#include "ft_printf.h"

int	main()
{
	void	*ptr[103];

	for (int i = 0; i < 100; i++)
		ptr[i] = malloc(TINY_MAX - 1);
	free(ptr[98]);
	malloc(800);
	size_t	len = 768;
	char	*p = malloc(len);
	ft_printf("%p\n", p);
	if (!p)
		return 0;
	ft_memset(p, 42, len);
}
