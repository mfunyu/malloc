#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

void	error_exit(void)
{
	char	*err;

	err = strerror(errno);
	ft_putendl_fd(err, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	error_print_exit(char *msg)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	error_exit();
}
