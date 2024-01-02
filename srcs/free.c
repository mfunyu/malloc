#ifdef __APPLE__
#include <malloc/malloc.h>
#endif
#include "malloc_internal.h"
#include "lists.h"
#include "freelist.h"
#include "ft_printf.h"
#include <sys/mman.h>

static void	_free_alloc(t_magazine *magazine, t_malloc_chunk *chunk)
{
	t_malloc_chunk	*next;

	chunk->size &= ~ALLOCED;
# ifdef BONUS
	chunk = consolidation(magazine, chunk);
# endif
	if (chunk != magazine->top)
		freelist_add(magazine->freelist, chunk);
	next = NEXTCHUNK(chunk);
	next->prev_size = CHUNKSIZE(chunk);
	next->size &= ~PREV_IN_USE;
}

static void	_free_mmap(t_mmap_chunk **alloced_lst, t_mmap_chunk *chunk)
{
	t_mmap_chunk	*lst;

	if (*alloced_lst == chunk)
	{
		*alloced_lst = chunk->fd;
		return ;
	}
	lst = *alloced_lst;
	while (lst && lst->fd != chunk)
		lst = lst->fd;
	if (lst->fd == chunk)
		lst->fd = chunk->fd;
	if (munmap(chunk, CHUNKSIZE(chunk)))
	{
		ft_printf("Error: munmap\n");
		return ;
	}
}

void	free_(void *ptr)
{
	t_malloc_chunk	*chunk;
	size_t			size;

	chunk = CHUNK(ptr);
	size = ALLOCSIZE(chunk);
	if (((uintptr_t)chunk & (TINY_QUANTUM - 1)))
	{
		ft_printf("ERRRROOORR");
		return;
	}
	if (IS_MAPPED(chunk))
		_free_mmap(&(g_malloc.large_allocations), (t_mmap_chunk *)chunk);
	else if (size <= TINY_MAX || ((uintptr_t)chunk & (SMALL_QUANTUM - 1)))
		_free_alloc(&(g_malloc.tiny_magazine), chunk);
	else if (!((uintptr_t)chunk & (SMALL_QUANTUM - 1)))
		_free_alloc(&(g_malloc.small_magazine), chunk);
	//else error
}

void	free(void *ptr)
{
	if (!ptr)
		return ;

#ifdef __APPLE__
	malloc_zone_t	*zone;

	zone = malloc_zone_from_ptr(ptr);
	if (zone) {
		malloc_zone_free(zone, ptr);
		return ;
	}
#endif
	free_(ptr);
}
