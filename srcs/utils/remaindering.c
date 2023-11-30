#include "malloc_internal.h"

static void	*_split_chunk(t_malloc_chunk *chunk, size_t new_size)
{
	t_malloc_chunk	*next;
	int				chunk_flags;

	next = (void *)chunk + new_size;
	next->size = CHUNKSIZE(chunk) - new_size;
	NEXTCHUNK(next)->prev_size = next->size;

	chunk_flags = GET_FLAGS(chunk);
	chunk->size = new_size;
	chunk->size |= chunk_flags;
	return (next);
}

void	*remaindering(t_malloc_chunk *current, size_t size_req, e_size type)
{
	size_t	min_chunksize;

	if (type == TINY)
		min_chunksize = MIN_CHUNKSIZE;
	else if (type == SMALL)
		min_chunksize = SMALL_QUANTUM;

	if (CHUNKSIZE(current) < size_req + min_chunksize)
		return (NULL);

	return (_split_chunk(current, size_req));
}
