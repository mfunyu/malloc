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

void	TestDiff(size_t original_size, size_t new_size, bool expect_same)
{
	void	*ptr = set_data(original_size, '*');
	void	*new_ptr;

	new_ptr = ft_realloc(ptr, new_size);
	if (expect_same)
		EXPECT_EQ(ptr, new_ptr) << "should not change the address";
	else
		EXPECT_NE(ptr, new_ptr) << "should change the address";
}


TEST(ReallocTest, SameSize) {
	TestDiff(12, 12, true);
	TestDiff(1234, 1234, true);
}

TEST(ReallocTest, SameAlignedSize) {
	int		size = 4 * MALLOC_ALIGNMENT;
	TestDiff(size + 2, size + MALLOC_ALIGNMENT, true);
}

TEST(ReallocTest, EdgeSize) {
	TestDiff(12, 16, true);
}

TEST(ReallocTest, DifferentSize) {
	TestDiff(12, 17, false);
}

TEST(ReallocTest, NullZero) {
	void	*ptr = NULL;
	void	*new_ptr;

	new_ptr = ft_realloc(NULL, 0);
	EXPECT_EQ(ptr, new_ptr) << "should return NULL";
}
