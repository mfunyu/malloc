#include "malloc_internal.h"
#include "libft.h"
#include <errno.h>

void	*calloc(size_t nmemb, size_t size)
{
	void	*ptr;

	if (nmemb == 0 || size == 0)
	{
		nmemb = 1;
		size = 1;
	}
	ptr = malloc_(nmemb * size);
	if (ptr == NULL)
	{
		errno = ENOMEM;
		return (NULL);
	}
	ft_memset(ptr, 0, nmemb * size);
	return (ptr);
}
