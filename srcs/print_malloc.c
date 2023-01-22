#include <unistd.h>
#include "malloc.h"
#include "ft_printf.h"

void print_heap(void)__attribute__((destructor));

void	print_line(size_t len)
{
	size_t	i;

	i = 0;
	while (i++ < len)
		ft_putchar_fd('-', STDOUT_FILENO);
}

void	print_single_line(size_t len)
{
	print_line(12);
	ft_putchar_fd('+', STDOUT_FILENO);
	print_line(4);
	ft_putchar_fd('+', STDOUT_FILENO);
	print_line(len + 2);
	ft_putchar_fd('+', STDOUT_FILENO);
	print_line(4);
	ft_putchar_fd('+', STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void	print_head_to_end(char* head, char* max)
{
	char	*ptr;
	ft_printf("head: %p\n", head);
	ft_printf("end : %p\n", max);

	ptr = head + WORD;
	while (SIZE(ptr))
	{
		print_single_line(ft_strlen(ptr));
		ft_printf("%p ", ptr - WORD);
		if (!ALLOC(ptr))
			ft_printf("%s", CYAN);
		ft_printf("| %d ", *HEADER(ptr));
		ft_printf("| %s ", ptr);
		ft_printf("| %d ", *FOOTER(ptr));
		ft_printf("| ");
		ft_printf("%s", RESET);
		ft_printf("(%u -> %u : %p)\n", SIZE(ptr), SIZE(ptr) + DWORD, SIZE(ptr) + DWORD);
		print_single_line(ft_strlen(ptr));
		ptr = NEXTPTR(ptr);
	}
}

void	print_heap()
{
	ft_printf("TINY: %p ~ %p (%d bytes)\n", g_tiny_head, g_tiny_max, g_tiny_max - g_tiny_head);
	print_head_to_end(g_tiny_head, g_tiny_max);
	ft_printf("SMALL: %p ~ %p (%d bytes)\n", g_small_head, g_small_max, g_small_max - g_small_head);
	print_head_to_end(g_small_head, g_small_max);
	ft_printf("LARGE: %p ~ NA (NA)\n", g_large_head);
	// print_head_to_end(g_large_head, NULL);
}
