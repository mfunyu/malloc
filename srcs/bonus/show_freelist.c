#include "malloc_internal.h"
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

int	largebin_index_size(size_t index)
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

int get_size_by_index(size_t index)
{
	size_t	size;

	if (index < 64)
		size = index << 4;
	else
		size = largebin_index_size(index);
	return (size);
}

void	show_freelist(t_magazine magazine)
{
	t_malloc_chunk	*lst;
	size_t			size;

	ft_printf("< FreeList >\n");

	print_single_line();
	for (size_t i = 2; i < 128; i++)
	{
		lst = magazine.freelist[i];
		size = get_size_by_index(i);
		ft_printf(" [%i]", i);
		ft_printf(" (%i) |", size);
		while (lst)
		{
			ft_printf("-> %p |", lst);
			if (size != CHUNKSIZE(lst))
				D(CHUNKSIZE(lst));
			lst = lst->fd;
		}
		ft_printf("\n");
		print_single_line();
	}
	ft_printf("\n");
}
