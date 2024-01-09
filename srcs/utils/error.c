#include "malloc_internal.h"
#include "ft_printf.h"

void	*error_null(char *error_msg)
{
	ft_printf("Error: %s\n", error_msg);
	return (NULL);
}

int		error_ret(char *error_msg, int ret)
{
	ft_printf("Error: %s\n", error_msg);
	return (ret);
}

void	error_msg(char *error_msg)
{
	ft_printf("Error: %s\n", error_msg);
}
