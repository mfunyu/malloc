#ifndef DEBUG_H
# define DEBUG_H

# include "ft_printf.h"

# define S(str) ft_printf("=========== %s ===========\n", str)
# define P(ptr) ft_printf("%p\n", ptr)
# define D(str) ft_printf("<< %d >>\n", str)

# define SD(str, val) ft_printf("%s : %d\n", str, val)
# define SP(str, ptr) ft_printf("%s : %p\n", str, ptr)

#endif /* DEBUG_H */
