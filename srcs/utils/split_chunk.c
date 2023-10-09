#include "malloc.h"

void	*split_chunk(t_malloc_chunk *chunk, size_t new_size)
{
	t_malloc_chunk	*next;
	int				chunk_flags;

	next = (void *)chunk + new_size;
	next->size = CHUNKSIZE(chunk) - new_size;
	
	chunk_flags = GET_FLAGS(chunk);
	chunk->size = new_size;
	chunk->size |= chunk_flags;
	return (next);
}
