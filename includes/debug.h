#ifndef DEBUG_H
#define DEBUG_H

# define P(ptr) ft_printf("%p\n", ptr)
# define S(str) ft_printf("=========== %s ===========\n", str)
# define D(str) ft_printf("<< %d >>\n", str)
# define SD(str, val) ft_printf("%s : %d\n", str, val)

#endif /* DEBUG_H */
