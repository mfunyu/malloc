#include <gtest/gtest.h>
#include <stdio.h>
#include <dlfcn.h>
#include "../includes/malloc.h"

void	*handle_realloc;

void	*ft_malloc(size_t size);
void	ft_free(void *ptr);

void	*ft_realloc(void *ptr, size_t size)
{
	static void	*(*my_realloc)(void *, size_t);

	if (!my_realloc)
	{
		handle_realloc = dlopen("../../libft_malloc.so", RTLD_LOCAL | RTLD_LAZY);
		if (!handle_realloc) {
			exit(EXIT_FAILURE);
		}
		*(void **) (&my_realloc) = dlsym(handle_realloc, "realloc");
		printf("===== realloc loaded =====\n");
	}
	return my_realloc(ptr, size);
}

char	*set_data(size_t size, int chr)
{
	char *ptr;

	ptr = (char *)ft_malloc(size);
	if (!ptr)
		return (NULL);
	memset(ptr, chr, size - 1);
	ptr[size - 1] = '\0';

	return ptr;
}


/* -------------------------------------------------------------------------- */
/*                             Realloc Tiny Tests                             */
/* -------------------------------------------------------------------------- */

TEST(ReallocSameSizeTest, One) {
	void	*ptr = set_data(12, '*');
	void	*new_ptr;

	new_ptr = ft_realloc(ptr, 12);
	EXPECT_EQ(ptr, new_ptr) << "should not change the address";
}
