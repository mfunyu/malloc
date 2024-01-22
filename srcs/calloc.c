#include "malloc_internal.h"
#include "libft.h"
# ifdef BONUS
#  include "debug.h"
# endif
#include <errno.h>

void	*calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	total_size;

# ifdef BONUS
	alloc_debug(__func__, __builtin_return_address(0), size);
# endif
	total_size = nmemb * size;
	if (total_size == 0)
		total_size = 1;
	ptr = malloc_(total_size);
	if (ptr == NULL)
	{
		errno = ENOMEM;
		return (NULL);
	}
	ft_memset(ptr, 0, nmemb * size);
# ifdef BONUS
	debug_result(ptr);
# endif
	return (ptr);
}
