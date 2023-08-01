#include <unistd.h>
#include "malloc.h"
#include "ft_printf.h"

void show_alloc_heap(void)__attribute__((destructor));

void	print_line(size_t len, bool sep)
{
	size_t	i;
	char	c;

	c = '-';
	if (sep)
		c = '=';
	i = 0;
	while (i++ < len)
		ft_putchar_fd(c, STDOUT_FILENO);
}

void	print_single_line()
{
	print_line(22, false);
	ft_putstr_fd("++", STDOUT_FILENO);
	print_line(24, false);
	ft_putstr_fd("+\n", STDOUT_FILENO);
}

void	print_double_line()
{
	print_line(22, true);
	ft_putstr_fd("++", STDOUT_FILENO);
	print_line(24, true);
	ft_putstr_fd("+\n", STDOUT_FILENO);
}

void	print_head_to_end(t_malloc_chunk* head, t_malloc_chunk* tail)
{
	t_malloc_chunk	*chunk;
	t_malloc_chunk	*next;
	bool		is_prev_inuse;

	ft_printf("%p ~ %p (%d bytes)\n", head, tail, tail - head);
	print_double_line();
	chunk = head;
	int i = 0;
	while (chunk < tail && i < 4)
	{
		i++;
		next = (void *)chunk + chunk->size;
		is_prev_inuse = IS_ALLOCED(next->size);

		if (!IS_ALLOCED(chunk->size)) {
			ft_printf(" %20p ||", chunk->prev_size);
			ft_printf(" (%20p) | %d\n", chunk, sizeof(chunk->prev_size));
			print_single_line();
		}
		ft_printf(" %16p | %d ||", SIZE(chunk->size), IS_ALLOCED(chunk->size));
		ft_printf(" (%20p) |\n", &(chunk->size));
		print_single_line();
		if (is_prev_inuse) {
			size_t i = 0;
			while (i < ft_strlen(MEM(chunk))) {
				ft_printf(" %-20.8s ||\n", MEM(chunk) + i);
				i += 8;
			}
		} else {
			ft_printf(" %20p ||", chunk->fd);
			ft_printf(" (%20p) |\n", &(chunk->fd));
			print_single_line();
			ft_printf(" %20p ||", chunk->bk);
			ft_printf(" (%20p) |\n", &(chunk->bk));
		}

		chunk = next;
		print_double_line();
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
