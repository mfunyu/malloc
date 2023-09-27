#include "malloc.h"
#include "ft_printf.h"

void	print_large(t_mmap_chunk *lst)
{
	while (lst)
	{
		ft_printf("LARGE: %p", lst);
		ft_printf("%p ~ %p : %d bytes\n", MEM(lst), MEM(lst) + SIZE(lst), SIZE(lst));
		lst = lst->fd;
	}
}

void	print_simple(t_region region)
{
	t_heap_chunk	*chunk;

	chunk = region.head;
	ft_printf("%p\n", region.head);
	while ((void *)chunk < region.tail)
	{
		if (IS_ALLOCED(chunk))
			ft_printf("%p ~ %p : %d bytes\n", MEM(chunk), MEM(chunk) + SIZE(chunk), SIZE(chunk));
		chunk = NEXTCHUNK(chunk);
	}
}

void	show_alloc_mem()
{
	ft_printf("TINY: ");
	print_simple(g_regions.tiny_region);
	ft_printf("SMALL: ");
	print_simple(g_regions.small_region);
	print_large(g_regions.large_lst);
}
