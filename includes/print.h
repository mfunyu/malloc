#ifndef PRINT_H
# define PRINT_H


# define THICK "\033[1m"
# define CYAN "\033[1;36m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define GRAY "\033[2m"
# define RESET "\033[m"


void	print_line(char c);
void	print_first_col(void *ptr);
void	print_row_ptr(void *ptr, char *name);
void	print_row(void *ptr, char *content, char *name);

#endif /* PRINT_H */
