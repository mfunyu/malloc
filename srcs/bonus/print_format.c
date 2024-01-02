#include "ft_printf.h"
#include "libft.h"
#include "print.h"

void	print_n_chars(char c, size_t len)
{
	size_t	i;

	i = 0;
	while (i++ < len)
		ft_putchar_fd(c, FILENO);
}

void	print_line(char c)
{
	print_n_chars(c, 18);
	ft_putstr_fd("++", FILENO);
	print_n_chars(c, 26);
	ft_putendl_fd("+", FILENO);
}

void	print_first_col(void *ptr)
{
	if (ptr)
		ft_printf(" %-17p||", ptr);
	else
		ft_printf("%18s||", "");
}

void	print_row_ptr(void *ptr, char *name)
{
	print_first_col(ptr);

	ft_printf(" %24p | ", *(void **)ptr);

	if (name)
		ft_printf(name);
	ft_printf("\n");
}

void	print_row(void *ptr, char *content, char *name)
{
	print_first_col(ptr);

	if (!content)
		ft_printf(" %-24.8s | ", (char *)ptr);
	else
		ft_printf(" %-24s | ", content);

	if (name)
		ft_printf(name);
	ft_printf("\n");
}
