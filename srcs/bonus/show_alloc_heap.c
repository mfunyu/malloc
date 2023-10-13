#include "malloc.h"
#include "print.h"
#include "bonus.h"

void show_alloc_heap(void)__attribute__((destructor));

static void	_print_used(t_malloc_chunk *chunk)
{
	ft_printf("%s", CYAN);
	print_line('-');
	print_row(MEM(chunk), NULL, "mem");
	print_row(MEM(chunk) + 8, NULL, NULL);
	if (MIN_CHUNKSIZE < ALLOCSIZE(chunk))
		print_row(NULL, "(( abbriviated ))", NULL);
}

static void	_print_unused(t_malloc_chunk *chunk)
{
	size_t			i;

	print_line('-');
	print_row(&(chunk->fd), NULL, "fd");
	print_line('-');
	print_row(&(chunk->bk), NULL, "bk");
	if (CHUNKSIZE(chunk) > 32) {
		print_line('-');
		print_row((void *)&(chunk->bk) + 8, NULL, NULL);
		print_first_col(NULL);
		i = CHUNKSIZE(chunk) - 32;
		ft_printf(" [%10d (%9p)] |\n", i, i);
	}
}

static void	_print_header(t_malloc_chunk *chunk)
{
	if (IS_PREV_IN_USE(chunk))
		print_row(chunk, NULL, NULL);
	else
		print_row(chunk, NULL, "prev_size");
	print_line('-');
	ft_printf("%s", RESET);
	if (IS_FOOTER(chunk))
		ft_printf("%s", GRAY);
	print_first_col(&(chunk->size));
	ft_printf(" %8d (%9p) ", ALLOCSIZE(chunk), CHUNKSIZE(chunk));
	ft_printf("|%c|%c| size\n", IS_ALLOCED(chunk) ? 'A' : '-', IS_PREV_IN_USE(chunk) ? 'P' : '-');
}

static void	_print_footer(t_malloc_chunk *footer)
{
	if (!IS_PREV_IN_USE(footer))
		ft_printf("%s", GRAY);
	_print_header(footer);
	print_line('-');
	print_row(&(footer->fd), NULL, "fd");
	print_line('=');
	ft_printf("%s\n", RESET);
}

static void	_print_magazine(t_magazine magazine)
{
	void			*tail;
	t_malloc_chunk	*chunk;

	tail = (void *)magazine.regions + magazine.size;
	ft_printf("%p ~ %p (%zu bytes)\n", magazine.regions, tail, magazine.size);
	chunk = magazine.regions;
	print_line('=');
	while (!IS_FOOTER(chunk))
	{
		_print_header(chunk);
		if (!IS_ALLOCED(chunk))
			_print_unused(chunk);
		else
			_print_used(chunk);
		chunk = NEXTCHUNK(chunk);
		print_line('=');
	}
	_print_footer(chunk);
}

static void	_print_large(t_mmap_chunk *lst)
{
	while (lst && lst->fd)
	{
		print_line('=');
		print_first_col(lst);
		ft_printf(" %24p | fd\n", lst->fd);
		print_line('-');
		print_first_col(&(lst->size));
		ft_printf(" %12d (%9p) | size\n", ALLOCSIZE(lst), CHUNKSIZE(lst));
		print_line('-');
		print_row(MEM(lst), NULL, "mem");
		print_line('=');
		lst = lst->fd;
	}
}

void	show_alloc_heap()
{
	if (!malloc_show_heap)
		return ;
	ft_printf("TINY: ");
	_print_magazine(g_malloc.tiny_magazine);
	show_freelist(g_malloc.tiny_magazine);
	ft_printf("SMALL: ");
	_print_magazine(g_malloc.small_magazine);
	show_freelist(g_malloc.small_magazine);
	ft_printf("LARGE: \n");
	_print_large(g_malloc.large_allocations);
}
