#ifndef BONUS_H
# define BONUS_H

extern unsigned	malloc_show_heap;

void	set_flags_from_environment();

void	show_alloc_heap();
void	show_freelist(t_magazine magazine);

#endif /* BONUS_H */