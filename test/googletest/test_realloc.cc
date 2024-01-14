#include <gtest/gtest.h>
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include "../includes/malloc_internal.h"

void	*ft_malloc(size_t size);
void	ft_free(void *ptr);

/* -------------------------------------------------------------------------- */
/*                                load realloc                                */
/* -------------------------------------------------------------------------- */

void	*handle_realloc;
void	*ft_realloc(void *ptr, size_t size)
{
	static void	*(*my_realloc)(void *, size_t);

	if (!my_realloc)
	{
		handle_realloc = dlopen("../../../libft_malloc.so", RTLD_LOCAL | RTLD_LAZY);
		if (!handle_realloc) {
			exit(EXIT_FAILURE);
		}
		*(void **) (&my_realloc) = dlsym(handle_realloc, "realloc");
		printf("===== realloc loaded =====\n");
	}
	return my_realloc(ptr, size);
}

/* -------------------------------------------------------------------------- */
/*                                close handler                               */
/* -------------------------------------------------------------------------- */

static void _exit_close(void)__attribute__((destructor));

static void	_exit_close(void)
{
	dlclose(handle_realloc);
}

/* -------------------------------------------------------------------------- */
/*                              support functions                             */
/* -------------------------------------------------------------------------- */

static void	_print_test_name(const char *category, const char *name)
{
	static char	*current_category;

	if (current_category != category)
	{
		fprintf(stderr, "\n==== %s ====\n", category);
		current_category = (char *)category;
	}
	fprintf(stderr, "[ %s ]\n", name);
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


char	*set_random_data(size_t size)
{
	char *ptr;

	ptr = (char *)ft_malloc(size);
	if (!ptr)
		return (NULL);
	for (int i; i < size - 1; i++)
	{
		char c = rand() & 127;
		c = c > 32 ? c : 32;
		ptr[i] = c;
	}
	ptr[size - 1] = '\0';

	return ptr;
}

int		size_generator()
{
	int size = rand() & 1023;
	if (size > TINY_MAX)
		size = TINY_MAX;
	return size;
}

/* -------------------------------------------------------------------------- */
/*                             Realloc Tiny Tests                             */
/* -------------------------------------------------------------------------- */

void	TestDiff(size_t original_size, size_t new_size, bool expect_same)
{
	void	*ptr = set_data(original_size, '*');
	void	*new_ptr;
	//ASSERT_EQ(NULL, ptr);

	new_ptr = ft_realloc(ptr, new_size);
	if (expect_same)
		EXPECT_EQ(ptr, new_ptr) << "should not change the address";
	else
		EXPECT_NE(ptr, new_ptr) << "should change the address";
}

void	TestBlocked(size_t original_size, size_t new_size)
{
	void	*ptr = set_data(original_size, '*');
	void	*new_ptr;
	ft_malloc(original_size);

	new_ptr = ft_realloc(ptr, new_size);
	EXPECT_NE(ptr, new_ptr) << "should change the address";
}


TEST(ReallocTest, SameSize) {
	_print_test_name("ReallocTest", "SameSize");
	TestDiff(12, 12, true);
	TestDiff(1234, 1234, true);
}

TEST(ReallocTest, SameAlignedSize) {
	_print_test_name("ReallocTest", "SameAlignedSize");
	int		size = 4 * MALLOC_ALIGNMENT;
	TestDiff(size + 2, size + MALLOC_ALIGNMENT, true);
}

TEST(ReallocTest, EdgeSize) {
	_print_test_name("ReallocTest", "EdgeSize");
	TestDiff(12, 16, true);
}

TEST(ReallocTest, Expanding) {
	_print_test_name("ReallocTest", "Expanding");
	TestDiff(12, 17, true);
}

TEST(ReallocTest, Realloced) {
	_print_test_name("ReallocTest", "Realloced");
	TestBlocked(12, 32);
}

TEST(ReallocTest, NullZero) {
	_print_test_name("ReallocTest", "NullZero");
	void	*ptr = NULL;
	void	*new_ptr;

	new_ptr = ft_realloc(NULL, 0);
	EXPECT_EQ(ptr, new_ptr) << "should return NULL";
}

TEST(ReallocTest, ReserveContents) {
	_print_test_name("ReallocTest", "ReserveContents");
	for (int i = 0; i < 100; i++)
	{
		void	*str= set_random_data(size_generator());
		if (!str)
			return ;
		char	*str1 = strdup((char *)str);
		char	*str2 = (char *)ft_realloc(str, size_generator());
		
		EXPECT_EQ(strcmp(str1, str2), 0);
	}
}
