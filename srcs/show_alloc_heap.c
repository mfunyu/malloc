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

void	print_head_to_end(t_malloc_chunk* head, t_malloc_chunk* tail)
{
	t_malloc_chunk	*chunk;
	void	*mem;

	ft_printf("%p ~ %p (%d bytes)\n", head, tail, tail - head);
	chunk = head;
	int i = 0;
	while (chunk < tail && i < 4)
	{
		i++;
		mem = MEM(chunk);
		print_single_line(ft_strlen(mem));

		ft_printf(" %p ", chunk);
		ft_printf("| %s ", mem);
		ft_printf("| %d ", ft_strlen(mem));
		ft_printf("| (%u -> %p)\n", chunk->size, chunk->size);
		print_single_line(ft_strlen(mem));
		chunk = (void *)chunk + chunk->size;
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
	print_head_to_end(tiny.blocks, (void *)tiny.blocks + tiny.map_size);
	show_free_list(tiny);
	ft_printf("SMALL: ");
	print_head_to_end(small.blocks, (void *)small.blocks + tiny.map_size);
	ft_printf("LARGE: %p ~ NA (NA)\n", large.blocks);
	// print_head_to_end(g_large_head, NULL);
}
