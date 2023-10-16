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

	col_width = 16;
	print_line(col_width);
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
	t_malloc_chunk	*freelist;

	ft_printf("< FreeList >\n");

	freelist = magazine.freelist;
	print_single_line();
	while (freelist) {
		ft_printf(" %p |", freelist);
		ft_printf(" %5d (%6p) |", ALLOCSIZE(freelist), ALLOCSIZE(freelist));
		ft_printf(" %14p |", freelist->fd);
		ft_printf(" %14p |", freelist->bk);
		ft_printf("\n");
		print_single_line();
		freelist = freelist->fd;
	}
	ft_printf("\n");
}
