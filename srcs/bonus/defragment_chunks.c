#include "malloc.h"
#include "freelist.h"

static void	*_merge_prev(t_magazine	*magazine, t_malloc_chunk *chunk)
{
	t_malloc_chunk	*prev;

	prev = PREVCHUNK(chunk);
	prev->size += CHUNKSIZE(chunk);
	if (chunk == magazine->top)
		magazine->top = prev;
	freelist_pop(magazine->freelist, prev);
	return (prev);
}

static void	_merge_next(t_magazine	*magazine, t_malloc_chunk *chunk)
{
	t_malloc_chunk	*next;

	next = NEXTCHUNK(chunk);
	chunk->size += CHUNKSIZE(next);
	if (next == magazine->top)
		magazine->top = chunk;
	else
		freelist_pop(magazine->freelist, next);
}

t_malloc_chunk	*defragment_chunks(t_magazine *magazine, t_malloc_chunk *chunk)
{
	if (!IS_ALLOCED(NEXTCHUNK(chunk)))
		_merge_next(magazine, chunk);
	if (!IS_PREV_IN_USE(chunk))
		chunk = _merge_prev(magazine, chunk);
	return (chunk);
}
