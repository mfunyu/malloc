

int		get_page_size()
{
	int	page_size;

	page_size = getpagesize();
	if (page_size == -1) {
		ft_putendl_fd("Error: getpagesize", STDERR_FILENO);
		return (-1);
	}
	return page_size;
}
