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
	print_n_chars(18, c);
	ft_putstr_fd("++", FILENO);
	print_n_chars(26, c);
	ft_putendl_fd("+", FILENO);
}

void	print_row(void *ptr, char *content, char *name)
{
	if (ptr)
		ft_printf("%18p||", ptr);
	else
		ft_printf("%18p||", "");

	if (!content)
		ft_printf(" %-24.8s | ", ptr);
	else
		ft_printf(" %-24s | ", content);

	if (name)
		ft_printf(name);
	ft_printf("\n");
}
