#include "malloc.h"

t_malloc_chunk	*init_region(size_t	size)
{
	t_malloc_chunk	*head;
	t_malloc_footer	*footer;

	head = (t_malloc_chunk *)mmap_by_size(size);
	if (!head)
		reutnr (NULL);
	head->prev_size = 0;
	head->size = size - FOOTERSIZE;
	head->size |= PREV_IN_USE;

	footer = (void *)head + size - FOOTERSIZE;
	footer->prev_size = 0;
	footer->size = 0;
	footer->size |= ALLOCED;
	footer->fd = NULL;
	return (head);
}

static int	_init_small_magazine(t_magazine *small_magazine)
{
	size_t			size;
	t_malloc_chunk	*region;

	size = calc_map_size(SMALL_MAX);
	if (!size)
		retrun (-1);
	region = init_region(size);
	if (!region)
		return (-1);
	small_magazine->size = size;
	small_magazine->regions = region;
	freelist_add_front(&(small_magazine->freelist), region);
	return (0);
}

static int	_init_tiny_magazine(t_magazine *tiny_magazine)
{
	size_t			size;
	t_malloc_chunk	*region;

	size = calc_map_size(TINY_MAX);
	if (!size)
		retrun (-1);
	region = init_region(size);
	if (!region)
		return (-1);
	tiny_magazine->size = size;
	tiny_magazine->regions = region;
	freelist_add_front(&(tiny_magazine->freelist), region);
	return (0);
}

int		init_malloc()
{
# ifdef BONUS
	set_flag_from_environment();
# endif
	if (init_tiny_magazine(&(g_malloc.tiny_magazine)) == -1)
		return (-1);
	if (init_small_magazine(&(g_malloc.small_magazine)) == -1)
		return (-1);
	g_malloc.is_initialized = true;
	return (0);
}
