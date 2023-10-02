#include <stddef.h>
#include "malloc.h"
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

void	*realloc(void *ptr, size_t size)
{
	if (size == 0)
		return (NULL);
	if (ptr == NULL)
		return (malloc(size));
	if (ALLOCSIZE(CHUNK(ptr)) >= size)
		return (ptr);
	return (ptr);
}
