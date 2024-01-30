#include "malloc_internal.h"
#include "ft_printf.h"

void	*print_error_ret_null(char *error_msg)
{
	ft_printf("Error: %s\n", error_msg);
	return (NULL);
}

int		print_error_ret_value(char *error_msg, int ret)
{
	ft_printf("Error: %s\n", error_msg);
	return (ret);
}

void	print_error_ret(char *error_msg)
{
	ft_printf("Error: %s\n", error_msg);
}
