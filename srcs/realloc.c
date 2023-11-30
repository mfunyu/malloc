#include "malloc.h"
#include "libft.h"
#include "utils.h"
#include "freelist.h"
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

void	extend_chunk(t_malloc_chunk *chunk, size_t size)
{
	t_malloc_chunk	*next;
	t_malloc_chunk	*rest;
	size_t			size_diff;
	size_t			new_chunk_size;
	t_magazine		*magazine;


	if (size <= TINY_MAX)
		new_chunk_size = align_malloc(size, TINY);
	else if (size <= SMALL_MAX)
		new_chunk_size = align_malloc(size, SMALL);
	next = NEXTCHUNK(chunk);
	size_diff = new_chunk_size - CHUNKSIZE(chunk); /* Always positive */
	if (size <= TINY_MAX)
		magazine = &(g_malloc.tiny_magazine);
	else
		magazine = &(g_malloc.small_magazine);
	if (next == magazine->top)
	{
		if (CHUNKSIZE(next) > size_diff + MIN_CHUNKSIZE)
		{
			rest = split_chunk(next, size_diff);
			magazine->top = rest;
		}
		else
			magazine->top = NULL;
	}
	else
	{
		freelist_pop(magazine->freelist, next);
		if (CHUNKSIZE(next) > size_diff + MIN_CHUNKSIZE)
		{
			rest = split_chunk(next, size_diff);
			freelist_add(magazine->freelist, rest);
		}
	}
	chunk->size += CHUNKSIZE(next);
	next = NEXTCHUNK(chunk);
	next->size |= PREV_IN_USE;
}

bool	is_chunk_extendable(t_malloc_chunk *chunk, size_t size)
{
	t_malloc_chunk	*next;
	size_t			new_chunk_size;

	if (size > TINY_MAX && (ALLOCSIZE(chunk) <= TINY_MAX
			|| !((uintptr_t)chunk & (SMALL_QUANTUM - 1)))) /* tiny to large */
		return (false);

	next = NEXTCHUNK(chunk);
	if (IS_ALLOCED(next))
		return (false);

	if (size <= TINY_MAX)
		new_chunk_size = align_malloc(size, TINY);
	else if (size <= SMALL_MAX)
		new_chunk_size = align_malloc(size, SMALL);
	if (new_chunk_size > CHUNKSIZE(chunk) + CHUNKSIZE(next))
		return (false);
	return (true);
}

void	*realloc_(void *ptr, size_t size)
{
	t_malloc_chunk	*chunk;
	void			*retval;

	if (size > MALLOC_ABSOLUTE_SIZE_MAX)
		return (NULL);
	chunk = CHUNK(ptr);
	if (ALLOCSIZE(chunk) >= size)
		return (ptr);

	if (size <= TINY_MAX)
	{
		if (is_chunk_extendable(chunk, size))
		{
			extend_chunk(chunk, size);
			return (ptr);
		}
	}
	else if (size <= SMALL_MAX)
	{
		if (is_chunk_extendable(chunk, size))
		{
			extend_chunk(chunk, size);
			return (ptr);
		}
	}
	retval = malloc_(size);
	//`SP("mall", ptr);
	if (!retval)
		return (NULL);
	ft_memmove(retval, ptr, ALLOCSIZE(chunk));
	free_(ptr);
	return (retval);
}


void	*realloc(void *ptr, size_t size)
{
	void	*retval;

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
	return (retval);
}
