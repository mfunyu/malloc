#include <stddef.h>

/*
** The realloc() function returns a pointer to the newly allocated memory,
** which is suitably aligned for any kind of variable and may be different from ptr,or NULL if the request fails.
** If size was equal to 0, either NULL or a pointer suitable to be passed to free() is returned.
** If realloc() fails the original block is left untouched; it is not freed or moved.
*/

void	*realloc(void *ptr, size_t size)
{
	if (size == 0)
		return (NULL);
	return (ptr);
}
