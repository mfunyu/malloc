#include "malloc.h"
#include "utils.h"
#include <stdlib.h>

int	largebin_index(size_t size)
{
	if (size >> 6 <= 48)
		return (48 + (size >> 6));
	if (size >> 9 <= 20)
		return (91 + (size >> 9));
	if (size >> 12 <= 10)
		return (110 + (size >> 12));
	if (size >> 15 <= 4)
		return (119 + (size >> 15));
	if (size >> 18 <= 2)
		return (124 + (size >> 18));
	return (126);
}

int	get_index_by_size(size_t size)
{
	int		index;

	if (size <= 1024)
		index = size >> 4;
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

void	freelist_add(t_malloc_chunk *freelist[128], t_malloc_chunk *add)
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

void	freelist_pop(t_malloc_chunk *freelist[128], t_malloc_chunk *pop)
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

void	*freelist_takeout(t_magazine *magazine, size_t size)
{
	int				index;
	t_malloc_chunk	*chunk;
	t_malloc_chunk	*next;

	index = get_index_by_size(size);
	if (!magazine->freelist[index])
		return (NULL);
	chunk = magazine->freelist[index];
	next = chunk->fd;
	if (next)
		next->bk = NULL;
	magazine->freelist[index] = next;
	next = remaindering(chunk, size, magazine->type);
	if (next)
		freelist_add(magazine->freelist, next);
	return (chunk);
}
