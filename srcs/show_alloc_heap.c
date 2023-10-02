#include <unistd.h>
#include "malloc.h"
#include "ft_printf.h"
#include "libft.h"
#include "print.h"
#include "flags.h"

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
	malloced_size = CHUNKSIZE(chunk) - HEADER_SIZE;
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
	if (WORD < CHUNKSIZE(chunk) - WORD) {
		print_line('-');
		print_first_column((void *)&(chunk->bk) + BYTE);
		ft_printf(" %-24s |\n", "");
		print_first_column(NULL);
		i = CHUNKSIZE(chunk) - WORD - WORD;
		ft_printf(" [%10d (%9p)] |\n", i, i);
	}
}

void	print_header(t_heap_chunk *chunk)
{
	print_first_column(chunk);
	if (IS_PREV_IN_USE(chunk))
		ft_printf(" %-24.8s |\n", chunk);
	else
		ft_printf(" %24p | prev_size\n", chunk->prev_size);
	print_line('-');
	if (IS_PREV_IN_USE(chunk))
		ft_printf("%s", RESET);
	print_first_column(&(chunk->size));
	ft_printf(" %6d (%7p) | %d | %d | size\n", ALLOCSIZE(chunk), CHUNKSIZE(chunk), (bool)IS_PREV_IN_USE(chunk), IS_ALLOCED(chunk));
}

void	print_footer(t_heap_chunk *chunk)
{
	t_footer_chunk	*footer;

	ft_printf("%s", GRAY);
	print_header(chunk);
	footer = (t_footer_chunk *)chunk;
	print_line('-');
	print_first_column(&(footer->next_chunk));
	ft_printf(" %24p | fd\n", footer->next_chunk);
	print_line('=');
	ft_printf("%s", RESET);
}

void	print_region(t_heap_chunk* head, t_heap_chunk* tail)
{
	t_heap_chunk	*chunk;

	chunk = head;
	ft_printf("%p ~ %p (%zu bytes)\n", head, tail, (size_t)tail - (size_t)head);
	print_line('=');
	while (!IS_FOOTER(chunk))
	{
		print_header(chunk);
		if (!IS_ALLOCED(chunk))
			print_unused(chunk);
		else
			print_used(chunk);
		chunk = NEXTCHUNK(chunk);
		print_line('=');
	}
	print_footer(chunk);
}

void	print_mmapped(t_mmap_chunk *lst)
{
	while (lst && lst->fd)
	{
		print_line('=');
		print_first_column(lst);
		ft_printf(" %24p | fd\n", lst->fd);
		print_line('-');
		print_first_column(&(lst->size));
		ft_printf(" %12d (%9p) | size\n", ALLOCSIZE(lst), CHUNKSIZE(lst));
		print_line('-');
		print_first_column(MEM(lst));
		ft_printf(" %-24.8s | mem\n", MEM(lst));
		print_line('=');
		lst = lst->fd;
	}
}

void	show_alloc_heap()
{
	t_region	tiny;
	t_region	small;

	if (!malloc_show_heap)
		return ;
	tiny = g_regions.tiny_region;
	small = g_regions.small_region;
	ft_printf("TINY: \n");
	print_region(tiny.head, (void *)tiny.tail);
	show_free_list(tiny);
	ft_printf("SMALL: \n");
	print_region(small.head, (void *)small.tail);
	show_free_list(small);
	ft_printf("LARGE: \n");
	print_mmapped(g_regions.large_lst);
}
