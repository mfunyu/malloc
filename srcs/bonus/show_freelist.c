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

static int	_smallbin_index_size(size_t index)
{
	if (index <= 31)
		return (1 << 9);
	if (index <= 36)
		return (1 << 10);
	if (index <= 41)
		return (1 << 11);
	if (index <= 44)
		return (1 << 12);
	if (index <= 48)
		return (1 << 13);
	if (index <= 52)
		return (1 << 14);
	if (index <= 55)
		return (1 << 15);
	if (index <= 59)
		return (1 << 16);
	return (1 << 17);
}

static int	_get_size_by_index(size_t index, e_size type)
{
	size_t	size;

	if (type == TINY)
		size = (index + 2) << 4;
	else if (type == SMALL)
		size = _smallbin_index_size(index);
	return (size);
}

void	show_freelist(t_magazine magazine)
{
	t_malloc_chunk	*lst;
	size_t			size;
	int				cnt;

	ft_printf("< FreeList >\n");

	_print_single_line();
	for (size_t i = 0; i < 64; i++)
	{
		lst = magazine.freelist[i];
		if (!lst)
			continue ;
		size = _get_size_by_index(i, magazine.type);
		ft_printf(" [%i]", i);
		ft_printf(" (%i) |", size);
		cnt = 0;
		while (lst)
		{
			ft_printf("-> %p (%zu)|", lst, CHUNKSIZE(lst));
			lst = lst->next;
			cnt++;
		}
		ft_printf(" (%d)\n", cnt);
		_print_single_line();
	}
	ft_printf("\n");
}
