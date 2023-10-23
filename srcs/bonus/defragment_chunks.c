#include "malloc.h"
#include "freelist.h"

t_malloc_chunk	*defragment_chunks(t_magazine *magazine, t_malloc_chunk *chunk)
{
	t_malloc_chunk	*next;
	t_malloc_chunk	*prev;

	if (!IS_ALLOCED(NEXTCHUNK(chunk)))
	{
		next = NEXTCHUNK(chunk);
		chunk->size += CHUNKSIZE(next);
		if (next == magazine->top)
			magazine->top = chunk;
		else
			freelist_pop(magazine->freelist, next);
	}
	if (!IS_PREV_IN_USE(chunk))
	{
		prev = PREVCHUNK(chunk);
		prev->size += CHUNKSIZE(chunk);
		if (chunk == magazine->top)
			magazine->top = prev;
		freelist_pop(magazine->freelist, prev);
		chunk = prev;
	}
	return (chunk);
}
