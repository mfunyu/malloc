#include "malloc_internal.h"
#include "init.h"
# ifdef BONUS
#  include "debug.h"
# endif
#include <errno.h>

/*
 [chunk utilise]
   chunk-> +-----------------------+--------
		   | size of prev chunk    | 8     ↑
		   +-----------------------+       |
		   | size              |A|P| 8     |chunk
	 ptr-> +=======================+====== |
		   |                       |     ↑ |
		   |    ~~~ alloced ~~~    |  ptr| |
 		   |                       |     | ↓
nxtchunk-> +-----------------------+-----|--
		   |                       | 8   ↓
		   +=======================+======

 [chunk free]
   chunk-> + ----------------------+--------
		   | size of prev chunk    | 8     ↑
		   +-----------------------+       |
		   | size              |A|P| 8     |chunk
		   +-----------------------+       |
		   | nextptr of free-lst   | 8     |
		   +-----------------------+       |
		   | prevptr of free-lst   | 8     |
		   +-----------------------+       |
		   |                       |       |
		   |    ~~~ alloced ~~~    |       |
		   |                       |       ↓
nxtchunk-> +-----------------------+--------
		   | size of prev chunk    | 8
		   +-----------------------+

 [last chunk]
   chunk-> +-----------------------+-----|--
		   |                       | ptr ↓ ↑
		   +=======================+====== |
		   | size = 0          |1|P| 8     |chunk
		   + ----------------------+       |
		   | ptr of next region    | 8     ↓
		   +-----------------------+--------
*/

/*
** for enable google testing, avoid a direct call to malloc within malloc, realloc, free functions
*/
void	*malloc_(size_t size)
{
	if (!size || size > MALLOC_ABSOLUTE_SIZE_MAX)
		return (NULL);
	if (!g_malloc.is_initialized)
	{
		if (init_malloc() == -1)
			return (NULL);
	}
	return (allocate(size));
}

void	*malloc(size_t size)
{
	void	*ptr;

# ifdef BONUS
	alloc_debug(__func__, __builtin_return_address(0), size);
# endif
	ptr = malloc_(size);
	if (ptr == NULL)
		errno = ENOMEM;
# ifdef BONUS
	debug_result(ptr);
# endif
	return (ptr);
}
