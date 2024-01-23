#include "malloc_internal.h"
#include "ft_printf.h"
#include "libft.h"

static void	_print_line(size_t len)
{
	size_t	i;

	i = 0;
	while (i++ < len)
		ft_putchar_fd('-', FILENO);
}

static void	_print_single_line()
{
	int	col_width;

	col_width = 18;
	_print_line(10);
	ft_putchar_fd('+', FILENO);
	_print_line(col_width);
	ft_putchar_fd('+', FILENO);
	_print_line(col_width);
	ft_putchar_fd('+', FILENO);
	_print_line(col_width);
	ft_putchar_fd('+', FILENO);
	ft_putchar_fd('\n', FILENO);
}

static int	_largebin_index_size(size_t index)
{
	if (index <= 97)
		return ((index - 48) << 6);
	if (index <= 112)
		return ((index - 91) << 9);
	if (index <= 120)
		return ((index - 110) << 12);
	if (index <= 124)
		return ((index - 119) << 15);
	return ((index - 124) << 18);
}

static int	_get_size_by_index(size_t index)
{
	size_t	size;

	if (index < 64)
		size = (index + 1) << 4;
	else
		size = _largebin_index_size(index);
	return (size);
}

void	show_freelist(t_magazine magazine)
{
	t_malloc_chunk	*lst;
	size_t			size;

	ft_printf("< FreeList >\n");

	_print_single_line();
	for (size_t i = 0; i < 128; i++)
	{
		lst = magazine.freelist[i];
		if (!lst)
			continue ;
		size = _get_size_by_index(i);
		ft_printf(" [%i]", i);
		ft_printf(" (%i) |", size);
		while (lst)
		{
			ft_printf("-> %p |", lst);
			lst = lst->fd;
		}
		ft_printf("\n");
		_print_single_line();
	}
	ft_printf("\n");
}
