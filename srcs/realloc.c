#include "malloc.h"
#include "libft.h"
#include "utils.h"
#include "lists.h"
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

bool	extend_chunk(t_malloc_chunk *chunk, size_t size)
{
	t_malloc_chunk	*next;
	t_malloc_chunk	*new;
	size_t			size_diff;
	t_magazine		*magazine;

	next = NEXTCHUNK(chunk);
	size_diff = align_malloc(size) - ALLOCSIZE(chunk);
	if (IS_ALLOCED(next) || size_diff > CHUNKSIZE(next))
		return (false);
	if (size <= TINY_MAX)
		magazine = &(g_malloc.tiny_magazine);
	else
		magazine = &(g_malloc.small_magazine);
	lst_malloc_chunk_pop(&(magazine->freelist), next);
	if (CHUNKSIZE(next) - size_diff > MIN_CHUNKSIZE)
	{
		new = split_chunk(next, size_diff);
		lst_malloc_chunk_sort_add(&(magazine->freelist), new);
	}
	chunk->size += CHUNKSIZE(next);
	next = NEXTCHUNK(chunk);
	next->size |= PREV_IN_USE;
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
	if (size <= SMALL_MAX)
	{
		if (extend_chunk(chunk, size))
			return (ptr);
	}
	retval = malloc_(size);
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
