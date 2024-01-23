#include "malloc_internal.h"
#include "utils.h"
#include <stdlib.h>

int	largebin_index(size_t size)
{
	if (size >> 9 <= 34)
		return ((size >> 9) + -3);
	if (size >> 10 <= 22)
		return ((size >> 10) + 14);
	if (size >> 11 <= 15)
		return ((size >> 11) + 25);
	if (size >> 12 <= 11)
		return ((size >> 12) + 33);
	if (size >> 13 <= 9)
		return ((size >> 13) + 39);
	if (size >> 14 <= 8)
		return ((size >> 14) + 44);
	if (size >> 15 <= 7)
		return ((size >> 15) + 48);
	if (size >> 16 <= 7)
		return ((size >> 16) + 52);
	if (size >> 17 <= 6)
		return ((size >> 17) + 56);
	return (63);
}

int	get_index_by_size(size_t size)
{
	int		index;

	if (size <= 1024 || size % SMALL_QUANTUM)
		index = (size >> 4) - 2;
	else
		index = largebin_index(size);
	return (index);
}

int	get_index(t_malloc_chunk *chunk)
{
	size_t	chunk_size;

	chunk_size = CHUNKSIZE(chunk);
	return (get_index_by_size(chunk_size));
}

void	freelist_add(t_malloc_chunk *freelist[64], t_malloc_chunk *add)
{
	int				index;
	t_malloc_chunk	*next;

	index = get_index(add);
	next = freelist[index];
	if (next)
		next->bk = add;
	add->fd = next;
	add->bk = NULL;
	freelist[index] = add;
}

void	freelist_pop(t_malloc_chunk *freelist[64], t_malloc_chunk *pop)
{
	t_malloc_chunk	*next;
	t_malloc_chunk	*prev;
	int				index;

	next = pop->fd;
	prev = pop->bk;
	if (next)
		next->bk = prev;
	if (prev)
		prev->fd = next;
	else
	{
		index = get_index(pop);
		freelist[index] = next;
	}
}

static t_malloc_chunk	*_find_first_hit(t_malloc_chunk *freelist, size_t size)
{
	t_malloc_chunk	*lst;

	lst = freelist;
	while (lst)
	{
		if (CHUNKSIZE(lst) >= size)
			return (lst);
		lst = lst->fd;
	}
	return (NULL);
}

void	*freelist_takeout(t_magazine *magazine, size_t size)
{
	int				index;
	t_malloc_chunk	*chunk;
	t_malloc_chunk	*remainder;

	index = get_index_by_size(size);
	
	if (magazine->type == TINY)
		chunk = magazine->freelist[index];
	else if (magazine->type == SMALL)
		chunk = _find_first_hit(magazine->freelist[index], size);
	if (!chunk)
		return (NULL);
	freelist_pop(magazine->freelist, chunk);
	remainder = remaindering(chunk, size, magazine->type);
	if (remainder)
		freelist_add(magazine->freelist, remainder);
	return (chunk);
}
