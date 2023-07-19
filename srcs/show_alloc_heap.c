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

void	print_head_to_end(void* head, void* max)
{
	void	*chunk_ptr;
	unsigned int  size;
	void	*mem;
	ft_printf("head: %p\n", head);
	ft_printf("end : %p\n", max);

	chunk_ptr = head;
	while (chunk_ptr < max)
	{
		mem = chunk_ptr + 16;
		size = *((unsigned int *)chunk_ptr);
		ft_printf("size: %d\n", size);
		print_single_line(ft_strlen(mem));

		ft_printf("  %p ", chunk_ptr);
		ft_printf("| %s ", mem);
		ft_printf("| %d ", ft_strlen(mem));
		ft_printf("| \n");
		//ft_printf("(%u -> %u : %p)\n", SIZE(chunk_ptr), SIZE(chunk_ptr) + DWORD, SIZE(chunk_ptr) + DWORD);
		print_single_line(ft_strlen(mem));
		chunk_ptr += size + 16;
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
	ft_printf("TINY: %p ~ %p (%d bytes)\n", tiny.head, tiny.tail, tiny.tail - tiny.head);
	print_head_to_end(tiny.head, tiny.tail);
	ft_printf("SMALL: %p ~ %p (%d bytes)\n", small.head, small.tail, small.tail - small.head);
	print_head_to_end(small.head, small.tail);
	ft_printf("LARGE: %p ~ NA (NA)\n", large.head);
	// print_head_to_end(g_large_head, NULL);
}
