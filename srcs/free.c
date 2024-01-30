#ifdef __APPLE__
#include <malloc/malloc.h>
#endif
#ifdef BONUS
# include "debug.h"
#endif
#include "malloc_internal.h"
#include "freelist.h"
#include "utils.h"
#include <sys/mman.h>

static void	_free_alloc(t_magazine *magazine, t_malloc_chunk *chunk)
{
	t_malloc_chunk	*next;

	if (!is_allocated_hint(chunk, magazine->type))
		return (print_error_ret("pointer being freed was not allocated"));
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
		*alloced_lst = chunk->next;
	else
	{
		lst = *alloced_lst;
		while (lst && lst->next != chunk)
			lst = lst->next;
		if (lst->next == chunk)
			lst->next = chunk->next;
		else
			return (print_error_ret("pointer being freed was not allocated"));
	}
	if (munmap(chunk, CHUNKSIZE(chunk)))
		return (print_error_ret("munmap failed"));
}

void	free_(void *ptr)
{
	t_malloc_chunk	*chunk;
	size_t			size;

	chunk = CHUNK(ptr);
	if (((uintptr_t)chunk & (TINY_QUANTUM - 1)))
		return (print_error_ret("Non-aligned pointer being freed"));
	size = CHUNKSIZE(chunk);
	if (IS_MAPPED(chunk))
		_free_mmap(&(g_malloc.large_allocations), (t_mmap_chunk *)chunk);
	else if (size <= TINY_CHUNKSIZE_MAX)
		_free_alloc(&(g_malloc.tiny_magazine), chunk);
	else if (!((uintptr_t)chunk & (SMALL_QUANTUM - 1)))
		_free_alloc(&(g_malloc.small_magazine), chunk);
	else
		return (print_error_ret("pointer being freed was not allocated"));
}

void	free(void *ptr)
{
	if (!ptr)
		return ;

#ifdef BONUS
	free_debug(__builtin_return_address(0), ptr);
#endif
#ifdef __APPLE__
	malloc_zone_t	*zone;

	zone = malloc_zone_from_ptr(ptr);
	if (zone) {
		malloc_zone_free(zone, ptr);
		return ;
	}
#endif
	free_(ptr);
#ifdef BONUS
	debug_result(NULL);
#endif
}
