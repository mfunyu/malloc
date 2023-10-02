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

static void	print_single_line(size_t len)
{
	print_line(16);
	ft_putchar_fd('+', FILENO);
	print_line(len + 3);
	ft_putchar_fd('+', FILENO);
	print_line(16);
	ft_putchar_fd('+', FILENO);
	print_line(16);
	ft_putchar_fd('+', FILENO);
	ft_putchar_fd('\n', FILENO);
}


void	show_free_list(t_region region)
{
    t_heap_chunk    *freelist;

    ft_printf("Chunk Free\n");

    freelist = region.freelist;
	print_single_line(10);
    while (freelist) {
        ft_printf(" %p | ", freelist);
        ft_printf("%3d (%5p) | ", freelist->size, freelist->size);
        ft_printf("%14p | ", freelist->fd);
        ft_printf("%14p | ", freelist->bk);
        ft_printf("\n");
		print_single_line(10);
		freelist = freelist->fd;
	}
    ft_printf("\n");
}
