#include <unistd.h>
#include "malloc.h"
#include "ft_printf.h"

void show_alloc_heap(void)__attribute__((destructor));

void	print_line(size_t len)
{
	size_t	i;

	i = 0;
	while (i++ < len)
		ft_putchar_fd('-', STDOUT_FILENO);
}

void	print_single_line(size_t len)
{
	print_line(16);
	ft_putchar_fd('+', STDOUT_FILENO);
	print_line(len + 2);
	ft_putchar_fd('+', STDOUT_FILENO);
	print_line(4);
	ft_putchar_fd('+', STDOUT_FILENO);
	ft_putchar_fd('\n', STDOUT_FILENO);
}

void	print_head_to_end(void* head, void* tail)
{
	void	*chunk_ptr;
	unsigned int  size;
	void	*mem;

	ft_printf("%p ~ %p (%d bytes)\n", head, tail, tail - head);
	chunk_ptr = head;
	while (chunk_ptr < tail)
	{
		mem = chunk_ptr + WORD;
		size = *((unsigned int *)chunk_ptr);
		print_single_line(ft_strlen(mem));

		ft_printf(" %p ", chunk_ptr);
		ft_printf("| %s ", mem);
		ft_printf("| %d ", ft_strlen(mem));
		ft_printf("| (%u -> %p)\n", size, size);
		print_single_line(ft_strlen(mem));
		chunk_ptr += size + WORD;
	}
}

void	show_alloc_heap()
{
	t_region	tiny;
	t_region	small;
	t_region	large;

	tiny = g_regions.tiny_region;
	small = g_regions.small_region;
	large = g_regions.large_region;
	ft_printf("TINY: ");
	print_head_to_end(tiny.head, tiny.tail);
	show_free_list(tiny);
	ft_printf("SMALL: ");
	print_head_to_end(small.head, small.tail);
	ft_printf("LARGE: %p ~ NA (NA)\n", large.head);
	// print_head_to_end(g_large_head, NULL);
}
