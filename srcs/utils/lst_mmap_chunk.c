#include "malloc.h"

void	lst_mmap_chunk_add(t_mmap_chunk **head, t_mmap_chunk *chunk)
{
	t_mmap_chunk	*lst;

	if (!*head)
	{
		*head = chunk;
		return ;
	}
	lst = *head;
	while (lst && lst->fd)
		lst = lst->fd;
	lst->fd = chunk;
}
