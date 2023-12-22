#ifndef BONUS_H
# define BONUS_H

extern unsigned	malloc_show_heap;
extern unsigned	malloc_show_abbr;

void	set_flags_from_environment();

void	show_freelist(t_magazine magazine);

void	print_chunk(t_malloc_chunk *chunk);

void	*extend_region(t_magazine *magazine);

t_malloc_chunk	*consolidation(t_magazine *magazine, t_malloc_chunk *chunk);

#endif /* BONUS_H */
