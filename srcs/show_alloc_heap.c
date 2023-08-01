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

void	print_single_line(bool line)
{
	print_line(18, line);
	ft_putstr_fd("++", STDOUT_FILENO);
	print_line(22, line);
	ft_putstr_fd("+\n", STDOUT_FILENO);
}

void	print_head(void *ptr, bool line)
{
	print_single_line(line);
	ft_printf("%15p   ||", ptr);
}

void	print_head_to_end(t_malloc_chunk* head, t_malloc_chunk* tail)
{
	t_malloc_chunk	*chunk;
	t_malloc_chunk	*next;
	bool		is_prev_inuse;

	ft_printf("%p ~ %p (%d bytes)\n", head, tail, tail - head);
	chunk = head;
	int i = 0;
	while (chunk < tail && i < 4)
	{
		i++;
		next = (void *)chunk + chunk->size;
		is_prev_inuse = IS_ALLOCED(next->size);

		if (!IS_ALLOCED(chunk->size)) {
			print_head(chunk, true);
			ft_printf(" %20p |", chunk->prev_size);
			ft_printf(" %d\n", sizeof(chunk->prev_size));
			print_head(&(chunk->size), false);
		} else {
			print_head(&(chunk->size), true);
		}
		ft_printf(" %16p | %d |\n", SIZE(chunk->size), IS_ALLOCED(chunk->size));
		if (is_prev_inuse) {
			size_t i = 0;
			while (i < ft_strlen(MEM(chunk))) {
				print_head(MEM(chunk) + i, false);
				ft_printf(" %-20.8s |\n", MEM(chunk) + i);
				i += 8;
			}
		} else {
			print_head(&(chunk->fd), false);
			ft_printf(" %20p |\n", chunk->fd);
			print_head(&(chunk->bk), false);
			ft_printf(" %20p |\n", chunk->bk);
		}

		chunk = next;
	}
	print_single_line(true);
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
