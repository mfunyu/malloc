#include "malloc_internal.h"
#include "libft.h"
#include "utils.h"
#include "freelist.h"
# ifdef BONUS
#  include "debug.h"
# endif
#include <errno.h>

/*
** The realloc() function returns a pointer to the newly allocated memory,
** which is suitably aligned for any kind of variable and may be different from ptr,or NULL if the request fails.
** - size == 0: either NULL or a pointer suitable to be passed to free() is returned.
** - size == 0 && ptr: the call is equivalent to free(ptr).
** - ptr == NULL: the call is equivalent to malloc(size), for all values of size.
** - ptr != NULL: it must have been returned by an earlier call to malloc(), calloc(), or  realloc().
** - fail: If realloc() fails the original block is left untouched; it is not freed or moved.
** - success: If the area pointed to was moved, a free(ptr) is done.
*/

static void	_extend_chunk(t_malloc_chunk *chunk, t_magazine *magazine, size_t chunk_size)
{
	t_malloc_chunk	*next;
	t_malloc_chunk	*remainder;
	size_t			size_diff;

	next = NEXTCHUNK(chunk);
	size_diff = chunk_size - CHUNKSIZE(chunk); /* Always positive */
	if (next == magazine->top)
	{
		magazine->top = remaindering(next, size_diff, magazine->type);
		if (!magazine->top)
			magazine->top = NEXTCHUNK(next);
	}
	else
	{
		freelist_pop(magazine->freelist, next);
		remainder = remaindering(next, size_diff, magazine->type);
		if (remainder)
			freelist_add(magazine->freelist, remainder);
	}
	chunk->size += CHUNKSIZE(next);
	next = NEXTCHUNK(chunk);
	next->size |= PREV_IN_USE;
}

static bool	_is_chunk_extendable(t_malloc_chunk *chunk, size_t new_size, size_t chunk_size)
{
	t_malloc_chunk	*next;

	if (new_size > TINY_MAX && CHUNKSIZE(chunk) <= TINY_BLOCKSIZE_MAX) /* tiny to small */
		return (false);

	next = NEXTCHUNK(chunk);
	if (IS_ALLOCED(next))
		return (false);
	if (chunk_size > CHUNKSIZE(chunk) + CHUNKSIZE(next))
		return (false);
	return (true);
}

static int	_realloc_check(t_magazine *magazine, t_malloc_chunk *chunk, size_t new_size)
{
	size_t	chunk_size;

	if (!_is_allocated(magazine, chunk))
		return (error_ret("pointer being realloc'd was not allocated", -1));
	if (ALLOCSIZE(chunk) >= new_size)
		return (true);
	chunk_size = align_malloc(new_size, magazine->type);
	if (_is_chunk_extendable(chunk, new_size, chunk_size))
	{
		_extend_chunk(chunk, magazine, chunk_size);
		return (true);
	}
	return (false);
}

void	*realloc_(void *ptr, size_t new_size)
{
	t_malloc_chunk	*chunk;
	void			*retval;
	int				ret;

	if (new_size > MALLOC_ABSOLUTE_SIZE_MAX)
		return (NULL);
	chunk = CHUNK(ptr);
	if (((uintptr_t)chunk & (TINY_QUANTUM - 1)))
		return (error_null("pointer being realloc'd was not allocated"));

	if (new_size <= TINY_MAX)
		ret = _realloc_check(&(g_malloc.tiny_magazine), chunk, new_size);
	else if (new_size <= SMALL_MAX)
		ret = _realloc_check(&(g_malloc.small_magazine), chunk, new_size);
	if (ret == true)
		return (ptr);
	else if (ret == -1)
		return (NULL);
	retval = malloc_(new_size);
	if (!retval)
		return (NULL);
	ft_memmove(retval, ptr, ALLOCSIZE(chunk));
	free_(ptr);
	return (retval);
}


void	*realloc(void *ptr, size_t size)
{
	void	*retval;

# ifdef BONUS
	realloc_debug(__builtin_return_address(0), ptr, size);
# endif
	if (ptr == NULL)
		retval = malloc_(size);
	else if (size == 0)
	{
		free_(ptr);
		return (NULL);
	}
	else
		retval = realloc_(ptr, size);
	if (retval == NULL)
		errno = ENOMEM;
# ifdef BONUS
	debug_result(retval);
# endif
	return (retval);
}
