#ifndef DEBUG_H
#define DEBUG_H

#include "ft_printf.h"

# define P(ptr) ft_printf("%p\n", ptr)
# define L(str) ft_printf("=========== %s ===========\n", str)

#endif /* DEBUG_H */
