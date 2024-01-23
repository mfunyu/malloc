#include "malloc_internal.h"
#include "freelist.h"
#include "utils.h"

static void	*_consolidate_prev(t_magazine	*magazine, t_malloc_chunk *chunk)
{
	t_malloc_chunk	*prev;

	prev = PREVCHUNK(chunk);
	freelist_pop(magazine->freelist, prev);
	prev->size += CHUNKSIZE(chunk);
	if (chunk == magazine->top)
		magazine->top = prev;
	return (prev);
}

static void	_consolidate_next(t_magazine	*magazine, t_malloc_chunk *chunk)
{
	t_malloc_chunk	*next;

	next = NEXTCHUNK(chunk);
	chunk->size += CHUNKSIZE(next);
	if (next == magazine->top)
		magazine->top = chunk;
	else
		freelist_pop(magazine->freelist, next);
}

t_malloc_chunk	*consolidation(t_magazine *magazine, t_malloc_chunk *chunk)
{
	size_t	max;

	max = 0;
	if (magazine->type == TINY)
		max = align_malloc(TINY_MAX, TINY);
	else if (magazine->type == SMALL)
		max = align_malloc(SMALL_MAX, SMALL);

	if (!IS_ALLOCED(NEXTCHUNK(chunk)) &&
			NEXTCHUNK(chunk)->size + CHUNKSIZE(chunk) <= max)
		_consolidate_next(magazine, chunk);
	if (!IS_PREV_IN_USE(chunk) &&
			PREVCHUNK(chunk)->size + CHUNKSIZE(chunk) <= max)
		chunk = _consolidate_prev(magazine, chunk);
	return (chunk);
}
