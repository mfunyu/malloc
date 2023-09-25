#include "malloc.h"

void	freelst_add_front(t_heap_chunk **lst, t_heap_chunk *new_chunk)
{
	new_chunk->fd = *lst;
	new_chunk->bk = NULL;
	if (*lst)
		(*lst)->bk = new_chunk;
	*lst = new_chunk;
}

void	freelst_insert(t_heap_chunk *prev, t_heap_chunk *new_chunk)
{
	new_chunk->bk = prev;
	new_chunk->fd = prev->fd;
	if (prev->fd)
		prev->fd->bk = new_chunk;
	prev->fd = new_chunk;
}

void	freelst_replace(t_heap_chunk *old, t_heap_chunk *new_chunk)
{
	new_chunk->fd = old->fd;
	new_chunk->bk = old->bk;
	if (old->bk)
		old->bk->fd = new_chunk;
	if (old->fd)
		old->fd->bk = new_chunk;
}

void	freelst_pop(t_heap_chunk *lst, t_heap_chunk **head)
{
	if (lst->bk)
		lst->bk->fd = lst->fd;
	if (lst->fd)
		lst->fd->bk = lst->bk;
	if (lst == *head)
		*head = lst->fd;
}