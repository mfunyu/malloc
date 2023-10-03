#include "ft_printf.h"
#include "libft.h"
#include "malloc.h"
#include "alloc.h"
#include <errno.h>

/*
[chunk utilise]
	chunk-> + ----------------------+ -------
			| size of prev chunk    | 8     ↑
			+ ----------------------+       |
			| size              |A|P| 8     |chunk
  ptr-> + ----------------------+ ===== |
			|                       |     ↑ |
			| 〜〜〜 alloced 〜〜〜   |  ptr| |
			|                       |     | ↓
 nxtchunk-> + ----------------------+ ----|--
			|                       | 8   ↓
			+ ----------------------+ =====

[chunk free]
	chunk-> + ----------------------+ -------
			| size of prev chunk    | 8     ↑
			+ ----------------------+       |
			| size              |A|P| 8     |chunk
			+ ----------------------+       |
			| nextptr of free-lst   | 8     |
			+ ----------------------+       |
			| prevptr of free-lst   | 8     |
			+ ----------------------+       |
			|                       |       |
			| 〜〜〜 alloced 〜〜〜   |       |
			|                       |       ↓
 nxtchunk-> + ----------------------+ -------
			| size of prev chunk    | 8
			+ ----------------------+

[last chunk]
	chunk-> + ----------------------+ ----|--
			|                       | ptr ↓ ↑
			+ ----------------------+ ===== |
			| size = 0          |1|P| 8     |chunk
			+ ----------------------+       |
			| ptr of next region    | 8     ↓
			+ ----------------------+ -------
*/

void	*malloc_(size_t size)
{
	ft_printf("malloc called %zu\n", size);
	if (!size || size > MALLOC_ABSOLUTE_SIZE_MAX)
		return (NULL);
	if (!g_regions.initialized && init_malloc() == ERROR)
		return (NULL);
	return (allocate_chunk(size));
}

void	*malloc(size_t size)
{
	void	*ptr;

	ptr = malloc_(size);
	if (ptr == NULL)
		errno = ENOMEM;
	return (ptr);
}
