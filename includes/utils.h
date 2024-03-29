#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>
# include <stdbool.h>

typedef struct	s_malloc_chunk t_malloc_chunk;
typedef struct	s_magazine t_magazine;
typedef enum	s_size e_size;

int		get_page_size();

void	*mmap_by_size(size_t map_size);

bool	is_allocated_hint(t_malloc_chunk *address, e_size hint);

/* alignment.c */
size_t	align_malloc(size_t size, e_size type);

void	*remaindering(t_malloc_chunk *current, size_t size_req, e_size type);

void	*extend_region(t_magazine *magazine);

/* error.c */
void	*print_error_ret_null(char *error_msg);
int		print_error_ret_value(char *error_msg, int ret);
void	print_error_ret(char *error_msg);

#endif /* UTILS_H */
