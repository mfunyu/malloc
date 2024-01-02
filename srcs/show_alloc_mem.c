#include "malloc_internal.h"
#include "ft_printf.h"

static size_t	_print_large_simple(t_mmap_chunk *lst)
{
	size_t	sum;

	sum = 0;
	while (lst)
	{
		ft_printf("LARGE : %p\n", lst);
		ft_printf("%p ~ %p : %zu bytes\n", MEM(lst), MEM(lst) + CHUNKSIZE(lst), CHUNKSIZE(lst));
		sum += CHUNKSIZE(lst);
		lst = lst->fd;
	}
	return (sum);
}

static size_t	_print_malloc_simple(char *zone, t_magazine magazine)
{
	t_malloc_chunk	*chunk;
	size_t			sum;
	size_t			size;

	sum = 0;
	chunk = magazine.regions;
	ft_printf("%s : %p\n", zone, chunk);
	while (!IS_FOOTER(chunk))
	{
		if (IS_ALLOCED(chunk))
		{
			size = ALLOCSIZE(chunk);
			ft_printf("%p ~ %p : %zu bytes\n", MEM(chunk), MEM(chunk) + size, size);
			sum += size;
		}
		chunk = NEXTCHUNK(chunk);
	}
	return (sum);
}

void	show_alloc_mem()
{
	size_t	total;

	total = 0;
	total += _print_malloc_simple("TINY", g_malloc.tiny_magazine);
	total += _print_malloc_simple("SMALL", g_malloc.small_magazine);
	total += _print_large_simple(g_malloc.large_allocations);
	ft_printf("TOTAL : %zu bytes\n", total);
}
