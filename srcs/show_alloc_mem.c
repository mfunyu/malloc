#include "malloc.h"
#include "ft_printf.h"

size_t	print_large(t_mmap_chunk *lst)
{
	size_t	sum;

	sum = 0;
	while (lst)
	{
		ft_printf("LARGE: %p\n", lst);
		ft_printf("%p ~ %p : %d bytes\n", MEM(lst), MEM(lst) + CHUNKSIZE(lst), CHUNKSIZE(lst));
		sum += CHUNKSIZE(lst);
		lst = lst->fd;
	}
	return (sum);
}

size_t	print_simple(char *zone, t_region region)
{
	t_heap_chunk	*chunk;
	size_t			sum;
	size_t			size;

	sum = 0;
	chunk = region.head;
	ft_printf("%s : %p\n", zone, region.head);
	while (!IS_FOOTER(chunk))
	{
		if (IS_ALLOCED(chunk))
		{
			size = CHUNKSIZE(chunk) - HEADER_SIZE;
			ft_printf("%p ~ %p : %d bytes\n", MEM(chunk), MEM(chunk) + size, size);
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
	total += print_simple("TINY", g_regions.tiny_region);
	total += print_simple("SMALL", g_regions.small_region);
	total += print_large(g_regions.large_lst);
	ft_printf("TOTAL: %zu bytes\n", total);
}
