#include <unistd.h>
#include "malloc.h"
#include "ft_printf.h"
#include "print.h"

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

void	print_first_column(void *ptr)
{
	if (ptr) {
		ft_printf("%15p   ||", ptr);
	} else {
		ft_printf("%15s   ||", "");
	}
}

void	print_mem(t_malloc_chunk *chunk)
{
	t_malloc_chunk	*next;
	bool			is_used;
	size_t			i;

	next = NEXTCHUNK(chunk);
	is_used = IS_PREV_IN_USE(next);

	if (is_used) {
		ft_printf("%s", CYAN);
		print_single_line(false);
		i = 0;
		while (i < SIZE(chunk) && i < WORD) {
			print_first_column(MEM(chunk) + i);
			ft_printf(" %-20.8s |", MEM(chunk) + i);
			if (!i)
				ft_printf(" mem");
			ft_printf("\n");
			i += BYTE;
		}
		if (i <= SIZE(chunk) - WORD) {
			print_first_column(NULL);
			ft_printf(" %-20s |\n", "(( abbriviated ))");
		}
	} else {
		print_single_line(false);
		print_first_column(&(chunk->fd));
		ft_printf(" %20p | fd\n", chunk->fd);
		print_single_line(false);
		print_first_column(&(chunk->bk));
		ft_printf(" %20p | bk\n", chunk->bk);
		if (WORD < SIZE(chunk) - WORD) {
			print_single_line(false);
			print_first_column((void *)&(chunk->bk) + BYTE);
			ft_printf(" %-20s |\n", "");
			print_first_column(NULL);
			i = SIZE(chunk) - WORD - WORD;
			ft_printf(" [%8d (%7p)] |\n", i, i);
		}
	}
}

void	print_region(t_malloc_chunk* head, t_malloc_chunk* tail)
{
	t_malloc_chunk	*chunk;

	int i = 0;
	chunk = head;
	ft_printf("%p ~ %p (%d bytes)\n", head, tail, tail - head);
	while (chunk < tail && ++i < 8)
	{
		print_single_line(true);
		print_first_column(chunk);
		if (IS_PREV_IN_USE(chunk)) {
			ft_printf(" %-20.8s |\n", chunk);
			print_single_line(false);
			ft_printf("%s", RESET);
		} else {
			ft_printf(" %20p | prev_size\n", chunk->prev_size);
			print_single_line(false);
		}
		print_first_column(&(chunk->size));
		ft_printf(" %6d (%7p) | %d | size\n", SIZE(chunk), SIZE(chunk), IS_PREV_IN_USE(chunk));
		print_mem(chunk);
		chunk = NEXTCHUNK(chunk);
	}
	print_single_line(true);
	ft_printf("%s", RESET);
}

void	show_alloc_heap()
{
	t_region	tiny;
	t_region	small;
	t_region	large;

	tiny = g_regions.tiny_region;
	small = g_regions.small_region;
	large = g_regions.large_region;
	ft_printf("TINY: \n");
	print_region(tiny.blocks, (void *)tiny.blocks + tiny.map_size);
	show_free_list(tiny);
	ft_printf("SMALL: ");
	print_region(small.blocks, (void *)small.blocks + tiny.map_size);
	ft_printf("LARGE: %p ~ NA (NA)\n", large.blocks);
	// print_head_to_end(g_large_head, NULL);
}
