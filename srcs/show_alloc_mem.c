#include "malloc_internal.h"
#include "ft_printf.h"
#include "utils.h"

static size_t	_print_large_simple(t_mmap_chunk *lst)
{
	size_t	sum;

	sum = 0;
	while (lst)
	{
		ft_printf("LARGE : %p\n", lst);
		ft_printf("%p ~ %p : %zu bytes\n", MEM(lst), MEM(lst) + CHUNKSIZE(lst), CHUNKSIZE(lst));
		sum += CHUNKSIZE(lst);
		lst = lst->next;
	}
	return (sum);
}

static size_t	_print_region_simple(t_malloc_chunk *region)
{
	t_malloc_chunk	*chunk;
	size_t			sum;
	size_t			size;

	sum = 0;
	chunk = region;
	while (!IS_FOOTER(chunk))
	{
		if (IS_ALLOCED(chunk))
		{
			size = ALLOCSIZE(chunk);
			ft_printf("%p ~ %p : %zu bytes\n", MEM(chunk), MEM(chunk) + size, size);
			sum += size;
		}
		if (chunk == NEXTCHUNK(chunk))
			break ;
		chunk = NEXTCHUNK(chunk);
	}
	return (sum);
}

static size_t	_print_malloc_simple(char *zone, t_magazine magazine)
{
	size_t	sum;
	t_malloc_chunk	*footer;

	sum = 0;
	for (t_malloc_chunk *region = magazine.regions; region; )
	{
		ft_printf("%s : %p\n", zone, region);
		sum += _print_region_simple(region);
		footer = (void *)region + magazine.size - REGION_FOOTERSIZE;
		region = footer->next;
	}
	return (sum);
}

void	show_alloc_mem()
{
	size_t	total;

	if (!g_malloc.is_initialized)
		return (error_msg("ShowAllocMem: malloc is not initialized")) ;
	total = 0;
	total += _print_malloc_simple("TINY", g_malloc.tiny_magazine);
	total += _print_malloc_simple("SMALL", g_malloc.small_magazine);
	total += _print_large_simple(g_malloc.large_allocations);
	ft_printf("TOTAL : %zu bytes\n", total);
}
