#include "malloc.h"

void	freelst_add_front(t_malloc_chunk **lst, t_malloc_chunk *new)
{
	new->fd = *lst;
	new->bk = NULL;
	if (*lst)
		(*lst)->bk = new;
	*lst = new;
}

void	freelst_insert(t_malloc_chunk *prev, t_malloc_chunk *new)
{
	new->bk = prev;
	new->fd = prev->fd;
	if (prev->fd)
		prev->fd->bk = new;
	prev->fd = new;
}

void	freelst_replace(t_malloc_chunk *old, t_malloc_chunk *new)
{
	new->fd = old->fd;
	new->bk = old->bk;
	if (old->bk)
		old->bk->fd = new;
	if (old->fd)
		old->fd->bk = new;
}

void	freelst_pop(t_malloc_chunk *lst)
{
	if (lst->bk)
		lst->bk->fd = lst->fd;
	if (lst->fd)
		lst->fd->bk = lst->bk;
}