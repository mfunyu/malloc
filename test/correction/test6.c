#include "malloc.h"
#include "ft_printf.h"
#include <string.h>

char *str = "The Zen of Python, by Tim Peters\n\
Beautiful is better than ugly.\n\
Explicit is better than implicit.\n\
Simple is better than complex.\n\
Complex is better than complicated.\n\
Flat is better than nested.\n\
Sparse is better than dense.\n\
Readability counts.\n\
Special cases aren't special enough to break the rules.\n\
Although practicality beats purity.\n\
Errors should never pass silently.\n\
Unless explicitly silenced.\n\
In the face of ambiguity, refuse the temptation to guess.\n\
There should be one-- and preferably only one --obvious way to do it.\n\
Although that way may not be obvious at first unless you're Dutch.\n\
Now is better than never.\n\
Although never is often better than *right* now.\n\
If the implementation is hard to explain, it's a bad idea.\n\
If the implementation is easy to explain, it may be a good idea.\n\
Namespaces are one honking great idea -- let's do more of those!\n\
";

void	malloc_realloc(char *title, size_t size)
{
	char	*before;
	char	*after;

	ft_printf("<malloc_realloc_%s>\n", title);
	before = malloc(strlen(str));
	strcpy(before, str);
	ft_printf("before: (%p) [%.50s...]\n", before, before);
	after = realloc(before, size);
	ft_printf("after : (%p) [%.50s...]\n", after, after);
	ft_printf("diff: %d\n", strcmp(str, after));
	ft_printf("\n");
}

int	main()
{
	size_t	base;
	ft_printf("\n===== test6 Basic Realloc Test: malloc -> realloc =====\n");

	base = strlen(str);
	malloc_realloc("minus", 10);
	malloc_realloc("plus1", base + 1);
	malloc_realloc("plus16", base + 16);
	malloc_realloc("x2", base * 2);
	malloc_realloc("small_zone", 1025);
	malloc_realloc("large_zone", 10000000000);

	show_alloc_mem();
}
