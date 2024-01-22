#include "malloc_internal.h"
#include "utils.h"

t_malloc	g_malloc;

static size_t	_calc_region_size(size_t max_block_size)
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

static int	_init_magazine(t_magazine *magazine, e_size type)
{
	size_t			size;
	t_malloc_chunk	*region;

	size = 0;
	if (type == TINY)
		size = _calc_region_size(TINY_MAX);
	else if (type == SMALL)
		size = _calc_region_size(SMALL_MAX);
	if (!size)
		return (-1);
	region = init_region(size);
	if (!region)
		return (-1);
	magazine->type = type;
	magazine->size = size;
	magazine->regions = region;
	magazine->top = region;
	return (0);
}

static int	_init_validations()
{
	if ((MALLOC_ALIGNMENT & (MALLOC_ALIGNMENT - 1)) != 0)
		return (error_ret("malloc alignment should be a power of two", -1));
	return (0);
}

int		init_malloc()
{
	if (_init_validations() == -1)
		return (-1);
# ifdef BONUS
	set_flags_from_environment();
# endif
	if (_init_magazine(&(g_malloc.tiny_magazine), TINY) == -1)
		return (-1);
	if (_init_magazine(&(g_malloc.small_magazine), SMALL) == -1)
		return (-1);
	g_malloc.is_initialized = true;
	return (0);
}
