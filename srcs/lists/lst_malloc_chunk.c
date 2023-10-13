#include "malloc.h"

void	lst_malloc_chunk_add_front(t_malloc_chunk **lst, t_malloc_chunk *new_chunk)
{
	new_chunk->fd = *lst;
	new_chunk->bk = NULL;
	if (*lst)
		(*lst)->bk = new_chunk;
	*lst = new_chunk;
}

void	lst_malloc_chunk_insert(t_malloc_chunk *prev, t_malloc_chunk *insert)
{
	insert->bk = prev;
	insert->fd = prev->fd;
	if (prev->fd)
		prev->fd->bk = insert;
	prev->fd = insert;
}

void	lst_malloc_chunk_replace(t_malloc_chunk **lst,
			t_malloc_chunk *old, t_malloc_chunk *new_chunk)
{
	new_chunk->fd = old->fd;
	new_chunk->bk = old->bk;
	if (old->bk)
		old->bk->fd = new_chunk;
	if (old->fd)
		old->fd->bk = new_chunk;
	if (old == *lst)
		*lst = new_chunk;
}

void	lst_malloc_chunk_pop(t_malloc_chunk **lst, t_malloc_chunk *pop)
{
	if (pop->bk)
		pop->bk->fd = pop->fd;
	if (pop->fd)
		pop->fd->bk = pop->bk;
	if (pop == *lst)
		*lst = pop->fd;
}

void	lst_malloc_chunk_sort_add(t_malloc_chunk **head, t_malloc_chunk *chunk)
{
	size_t			chunk_size;
	t_malloc_chunk	*lst;

	chunk_size = CHUNKSIZE(chunk);
	if (!*head || CHUNKSIZE((*head)) >= chunk_size)
		lst_malloc_chunk_add_front(head, chunk);
	else
	{
		lst = *head;
		while (lst->fd && CHUNKSIZE(lst->fd) < CHUNKSIZE(chunk))
			lst = lst->fd;
		lst_malloc_chunk_insert(lst, chunk);
	}
}