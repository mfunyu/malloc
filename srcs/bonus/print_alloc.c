#include "malloc_internal.h"
#include "print.h"
#include "bonus.h"

static void	_print_used(t_malloc_chunk *chunk)
{
	ft_printf("%s", CYAN);
	print_line('-');
	print_row(MEM(chunk), NULL, "mem");
	if (malloc_show_abbr)
		return ;
	print_row(MEM(chunk) + 8, NULL, NULL);
	if (MIN_CHUNKSIZE < ALLOCSIZE(chunk))
		print_row(NULL, "(( abbriviated ))", NULL);
}

static void	_print_unused(t_malloc_chunk *chunk)
{
	size_t			i;

	print_line('-');
	print_row_ptr(&(chunk->fd), "fd");
	print_line('-');
	print_row_ptr(&(chunk->bk), "bk");
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
	if (!IS_PREV_IN_USE(chunk))
	{
		print_row_ptr(chunk, "prev_size");
		print_line('-');
	}
	else if (!malloc_show_abbr)
	{
		print_row(chunk, NULL, NULL);
		print_line('-');
	}
	ft_printf("%s", RESET);
	if (IS_FOOTER(chunk))
		ft_printf("%s", GRAY);
	print_first_col(&(chunk->size));
	ft_printf(" %8d (%9p) ", ALLOCSIZE(chunk), CHUNKSIZE(chunk));
	ft_printf("|%c|%c| size\n", IS_ALLOCED(chunk) ? 'A' : '-', IS_PREV_IN_USE(chunk) ? 'P' : '-');
}

static void	_print_other_chunk(t_malloc_chunk *chunk, bool is_footer)
{
	if (is_footer && !IS_PREV_IN_USE(chunk))
		ft_printf("%s", GRAY);
	_print_header(chunk);
	print_line('-');
	if (is_footer)
		print_row_ptr(&(chunk->fd), "fd");
	else
		print_row(NULL, "(( abbriviated ))", NULL);
	print_line('=');
	if (is_footer)
		ft_printf("%s\n", RESET);
}

void	print_chunk(t_malloc_chunk *chunk)
{
	_print_header(chunk);
	if (!IS_ALLOCED(chunk))
		_print_unused(chunk);
	else
		_print_used(chunk);
	print_line('=');
}

void	print_malloc(t_magazine magazine, e_size type)
{
	void			*tail;
	t_malloc_chunk	*chunk;
	t_malloc_chunk	*region;

	region = magazine.regions;
	while (region)
	{
		tail = (void *)region + magazine.size;
		if (type == TINY)
			ft_printf("TINY: ");
		else if (type == SMALL)
			ft_printf("SMALL: ");
		ft_printf("%p ~ %p (%zu bytes)\n", region, tail, magazine.size);
		chunk = region;
		print_line('=');
		while (!IS_FOOTER(chunk) && chunk != magazine.top)
		{
			print_chunk(chunk);
			chunk = NEXTCHUNK(chunk);
		}
		if (chunk == magazine.top)
		{
			_print_other_chunk(chunk, false);
			chunk = NEXTCHUNK(chunk);
		}
		_print_other_chunk(chunk, true);
		region = chunk->fd;
	}
}

void	print_large(t_mmap_chunk *lst)
{
	ft_printf("LARGE: \n");
	while (lst)
	{
		print_line('=');
		print_first_col(lst);
		ft_printf(" %24p | fd\n", lst->fd);
		print_line('-');
		print_first_col(&(lst->size));
		ft_printf(" %12d (%9p) | size\n", ALLOCSIZE(lst), CHUNKSIZE(lst));
		if (!malloc_show_abbr)
		{
			print_line('-');
			print_row(MEM(lst), NULL, "mem");
		}
		print_line('=');
		lst = lst->fd;
	}
}

