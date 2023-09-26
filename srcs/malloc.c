#include "ft_printf.h"
#include "malloc.h"
#include "utils.h"
#include "alloc.h"
#include <unistd.h>
#include <stdio.h>

/*
[chunk utilise]
   chunk-> + ----------------------+ -------
           | size of prev chunk    | 8     ↑
           + ----------------------+       |
           | size              | P | 8     |chunk
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
           | size              | P | 8     |chunk
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
*/

void	*malloc(size_t size)
{
	ft_printf("malloc called %d\n", size);
	if (!size || size > MALLOC_ABSOLUTE_SIZE_MAX)
		return (NULL);
	if (!g_regions.initialized && init_malloc() == ERROR)
		return (NULL);
	return (allocate_chunk(size));
}
