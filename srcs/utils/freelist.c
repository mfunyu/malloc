#include "malloc.h"
#include "utils.h"

int	largebin_index(size_t size)
{
	if (size >> 6 <= 32)
		return (57 + (size >> 6));
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

	if (size <= 512)
		index = size / 8;
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
	int		index;

	index = get_index(add);
	add->fd = freelist[index];
	add->bk = NULL;
	freelist[index] = add;
}

void	freelist_pop(t_malloc_chunk *freelist[128], t_malloc_chunk *pop)
{
	int		index;

	if (!pop->bk)
	{
		index = get_index(pop);
		freelist[index] = pop->fd;
	}
	else
	{
		pop->bk->fd = pop->fd;
		if (pop->fd)
			pop->fd->bk = pop->bk;
	}
}

void	*freelist_takeout(t_malloc_chunk *freelist[128], size_t size)
{
	int				index;
	t_malloc_chunk	*chunk;
	t_malloc_chunk	*rest;

	index = get_index_by_size(size);
	if (!freelist[index])
		return (NULL);
	chunk = freelist[index];
	freelist[index] = chunk->fd;
	if (CHUNKSIZE(chunk) > MIN_CHUNKSIZE + size)
	{
		rest = split_chunk(chunk, size);
		freelist_add(freelist, rest);
	}
	return (chunk);
}
