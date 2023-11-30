#include "malloc.h"
#include "utils.h"
#include "lists.h"

t_malloc	g_malloc;

static size_t	calc_region_size(size_t max_block_size)
{
	size_t	region_size;
	size_t	page_size;

	page_size = get_page_size();
	if (!page_size)
		return (0);
	region_size = (max_block_size + CHUNK_OVERHEAD) * MIN_ALLOCNUMS + REGION_FOOTERSIZE;
	return (ALIGN(region_size, page_size));
}

t_malloc_chunk	*init_region(size_t	size)
{
	t_malloc_chunk	*head;
	t_malloc_footer	*footer;

	head = (t_malloc_chunk *)mmap_by_size(size);
	if (!head)
		return (NULL);
	head->prev_size = 0;
	head->size = size - REGION_FOOTERSIZE;
	head->size |= PREV_IN_USE;

	footer = (void *)head + size - REGION_FOOTERSIZE;
	footer->prev_size = CHUNKSIZE(head);
	footer->size = 0;
	footer->size |= ALLOCED;
	footer->fd = NULL;
	return (head);
}

static int	_init_small_magazine(t_magazine *small_magazine)
{
	size_t			size;
	t_malloc_chunk	*region;

	size = calc_region_size(SMALL_MAX);
	if (!size)
		return (-1);
	region = init_region(size);
	if (!region)
		return (-1);
	small_magazine->type = SMALL;
	small_magazine->size = size;
	small_magazine->regions = region;
	small_magazine->top = region;
	return (0);
}

static int	_init_tiny_magazine(t_magazine *tiny_magazine)
{
	size_t			size;
	t_malloc_chunk	*region;

	size = calc_region_size(TINY_MAX);
	if (!size)
		return (-1);
	region = init_region(size);
	if (!region)
		return (-1);
	tiny_magazine->type = TINY;
	tiny_magazine->size = size;
	tiny_magazine->regions = region;
	tiny_magazine->top = region;
	return (0);
}

int		init_validations()
{
	if ((MALLOC_ALIGNMENT & (MALLOC_ALIGNMENT - 1)) != 0)
		return (error_ret("configuration - malloc alignment should be a power of two", -1));
	return (0);
}

int		init_malloc()
{
	if (init_validations() == -1)
		return (-1);
# ifdef BONUS
	set_flags_from_environment();
# endif
	if (_init_tiny_magazine(&(g_malloc.tiny_magazine)) == -1)
		return (-1);
	if (_init_small_magazine(&(g_malloc.small_magazine)) == -1)
		return (-1);
	g_malloc.is_initialized = true;
	return (0);
}
