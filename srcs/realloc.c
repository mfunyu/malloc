#include <stddef.h>
#include "malloc.h"
#include "utils.h"
#include "alloc.h"
#include "libft.h"
#include "ft_printf.h"
/*
** The realloc() function returns a pointer to the newly allocated memory,
** which is suitably aligned for any kind of variable and may be different from ptr,or NULL if the request fails.
** If size was equal to 0, either NULL or a pointer suitable to be passed to free() is returned.
** If realloc() fails the original block is left untouched; it is not freed or moved.
*/

/*
** If ptr is NULL, then the call is equivalent to malloc(size), for all values of size;
** if size is equal to zero, and ptr is not NULL, then the call is equivalent to free(ptr).
** Unless ptr is NULL, it must have been returned by an earlier call to malloc(), calloc(), or  realloc().
** If the area pointed to was moved, a free(ptr) is done.
*/

void	*extend_chunk(t_heap_chunk *chunk, size_t size)
{
	size_t			extend_size;
	t_heap_chunk	*next;

	size = align(size, MALLOC_ALIGNMENT);
	extend_size = size - ALLOCSIZE(chunk);
	next = NEXTCHUNK(chunk);
	split_chunk(next, extend_size - HEADER_SIZE);
	chunk->size = size;
	freelst_replace(next, NEXTCHUNK(chunk), &(g_regions.tiny_region.freelist));
	next = NEXTCHUNK(chunk);
	next->size |= PREV_IN_USE;
	return (MEM(chunk));
}

bool	can_extend_chunk(t_heap_chunk *chunk, size_t size)
{
	t_heap_chunk	*next;

	next = NEXTCHUNK(chunk);
	if (IS_FOOTER(next))
		return (false);
	if (IS_ALLOCED(next))
		return (false);
	if (CHUNKSIZE(next) < align(size, MALLOC_ALIGNMENT) - ALLOCSIZE(chunk))
		return (false);
	return (true);
}

void	*handle_realloc(void *ptr, size_t size)
{
	t_heap_chunk	*chunk;
	void			*new_ptr;

	chunk = CHUNK(ptr);
	if (ALLOCSIZE(chunk) >= size)
		return (ptr);
	if (can_extend_chunk(chunk, size))
		return(extend_chunk(chunk, size));
	new_ptr = malloc_(size);
	if (!new_ptr)
		return (NULL);
	ft_strlcpy(new_ptr, ptr, size);
	free_(ptr);
	return (new_ptr);
}

void	*realloc(void *ptr, size_t size)
{
	void	*ret;

	ft_printf("realloc called: %p, %zu\n", ptr, size);
	if (ptr == NULL)
		return (malloc_(size));
	if (size == 0) {
		free_(ptr);
		return (NULL);
	}
	ret = handle_realloc(ptr, size);
	return (ret);
}
