#include "malloc.h"
#include "ft_printf.h"
#include "libft.h"

static void	print_line(size_t len)
{
	size_t	i;

	i = 0;
	while (i++ < len)
		ft_putchar_fd('-', FILENO);
}

static void	print_single_line()
{
	int	col_width;

	col_width = 18;
	print_line(10);
	ft_putchar_fd('+', FILENO);
	print_line(col_width);
	ft_putchar_fd('+', FILENO);
	print_line(col_width);
	ft_putchar_fd('+', FILENO);
	print_line(col_width);
	ft_putchar_fd('+', FILENO);
	ft_putchar_fd('\n', FILENO);
}


void	show_freelist(t_magazine magazine)
{
	t_malloc_chunk	*lst;

	ft_printf("< FreeList >\n");

	print_single_line();
	for (size_t i = 2; i < 128 ; i++)
	{
		lst = magazine.freelist[i];
		ft_printf(" [%i]", i);
		ft_printf(" (%i) |", i * 8);
		while (lst)
		{
			ft_printf("-> %p |", lst);
			if (i * 8 < 512 && i * 8 != CHUNKSIZE(lst))
				D(CHUNKSIZE(lst));
			lst = lst->fd;
		}
		ft_printf("\n");
		print_single_line();
	}
	ft_printf("\n");
}
