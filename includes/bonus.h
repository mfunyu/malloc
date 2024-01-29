#ifndef BONUS_H
# define BONUS_H

typedef enum e_flag
{
	SHOW_HEAP,
	SHOW_ABBR,
	SHOW_FREE,
	DEBUG,
	TOTAL
}			e_flag;

typedef struct	s_magazine t_magazine;
typedef struct	s_malloc_chunk t_malloc_chunk;

void	set_flags_from_environment();

void	show_freelist(t_magazine magazine);

void	print_chunk(t_malloc_chunk *chunk);

void	*extend_region(t_magazine *magazine);

t_malloc_chunk	*consolidation(t_magazine *magazine, t_malloc_chunk *chunk);

#endif /* BONUS_H */
