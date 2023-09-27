#include <unistd.h>
#include "malloc.h"
#include "ft_printf.h"
#include "print.h"

void show_alloc_heap(void)__attribute__((destructor));

void	print_chars(size_t len, char c)
{
	size_t	i;

	i = 0;
	while (i++ < len)
		ft_putchar_fd(c, FILENO);
}

void	print_line(char c)
{
	print_chars(18, c);
	ft_putstr_fd("++", FILENO);
	print_chars(26, c);
	ft_putstr_fd("+\n", FILENO);
}

void	print_first_column(void *ptr)
{
	if (ptr) {
		ft_printf("%15p   ||", ptr);
	} else {
		ft_printf("%15s   ||", "");
	}
}

void	print_used(t_heap_chunk *chunk)
{
	size_t	malloced_size;
	void	*address;

	ft_printf("%s", CYAN);
	print_line('-');
	malloced_size = SIZE(chunk) - HEADER_SIZE;
	address = MEM(chunk);
	print_first_column(address);
	ft_printf(" %-24.8s | mem\n", address);
	print_first_column(address + BYTE);
	ft_printf(" %-24.8s |\n", address + BYTE);
	if (MINSIZE < malloced_size) {
		print_first_column(NULL);
		ft_printf(" %-24s |\n", "(( abbriviated ))");
	}
}

void	print_unused(t_heap_chunk *chunk)
{
	size_t			i;

	print_line('-');
	print_first_column(&(chunk->fd));
	ft_printf(" %24p | fd\n", chunk->fd);
	print_line('-');
	print_first_column(&(chunk->bk));
	ft_printf(" %24p | bk\n", chunk->bk);
	if (WORD < SIZE(chunk) - WORD) {
		print_line('-');
		print_first_column((void *)&(chunk->bk) + BYTE);
		ft_printf(" %-24s |\n", "");
		print_first_column(NULL);
		i = SIZE(chunk) - WORD - WORD;
		ft_printf(" [%10d (%9p)] |\n", i, i);
	}
}

void	print_header(t_heap_chunk *chunk)
{
	print_line('=');
	print_first_column(chunk);
	if (IS_PREV_IN_USE(chunk))
		ft_printf(" %-24.8s |\n", chunk);
	else
		ft_printf(" %24p | prev_size\n", chunk->prev_size);
	print_line('-');
	ft_printf("%s", RESET);
	print_first_column(&(chunk->size));
	ft_printf(" %6d (%7p) | %d | %d | size\n", SIZE(chunk), SIZE(chunk), (bool)IS_PREV_IN_USE(chunk), IS_ALLOCED(chunk));
}

void	print_region(t_heap_chunk* head, t_heap_chunk* tail)
{
	t_heap_chunk	*chunk;

	chunk = head;
	ft_printf("%p ~ %p (%d bytes)\n", head, tail, tail - head);
	while (chunk < tail)
	{
		print_header(chunk);
		if (!IS_ALLOCED(chunk))
			print_unused(chunk);
		else
			print_used(chunk);
		chunk = NEXTCHUNK(chunk);
	}
	print_line('=');
}

void	show_alloc_heap()
{
	t_region	tiny;
	t_region	small;

	tiny = g_regions.tiny_region;
	small = g_regions.small_region;
	ft_printf("TINY: \n");
	print_region(tiny.head, (void *)tiny.tail);
	show_free_list(tiny);
	ft_printf("SMALL: ");
	print_region(small.head, (void *)small.tail);
	show_free_list(small);
	// print_head_to_end(g_large_head, NULL);
}
