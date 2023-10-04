#include <gtest/gtest.h>
#include <stdio.h>
#include <dlfcn.h>
#include "../includes/malloc.h"

void	*handle_malloc;
void	*handle_free;
void* ft_malloc(size_t size)
{
	static void	*(*my_malloc)(size_t);

	if (!my_malloc)
	{
		handle_malloc = dlopen("../../libft_malloc.so", RTLD_LOCAL | RTLD_LAZY);
		if (!handle_malloc) {
			exit(EXIT_FAILURE);
		}
		*(void **) (&my_malloc) = dlsym(handle_malloc, "malloc");
		printf("===== malloc loaded =====\n");
	}
	return my_malloc(size);
}

void ft_free(void *ptr)
{
	static void	(*my_free)(void *);

	if (!my_free)
	{
		handle_free = dlopen("../../libft_malloc.so", RTLD_LOCAL | RTLD_LAZY);
		if (!handle_free) {
			exit(EXIT_FAILURE);
		}
		*(void **) (&my_free) = dlsym(handle_free, "free");
		printf("===== free loaded =====\n");
	}
	return my_free(ptr);
}


char *set_data(void *(*func)(size_t), size_t size, int chr)
{
	char *ptr;

	ptr = (char *)func(size + 1);
	if (!ptr)
		return (NULL);
	memset(ptr, chr, size);
	ptr[size] = '\0';

	return ptr;
}

void	TestSimple(size_t len)
{
	void	*ac;
	void	*ex;

	ac = ft_malloc(len);
	ex = malloc(len);
	//EXPECT_EQ(ac, ex) << "malloc should reserve its contents";
//	free(ex);
}

void	TestOne(size_t len, bool check_free)
{
	char	*ac;
	char	*ex;
	char	chr = 'a';

	ac = set_data(ft_malloc, len, chr);
	ex = set_data(malloc, len, chr);
	EXPECT_EQ(strncmp(ac, ex, len), 0) << "malloc should reserve its contents";
	free(ex);
	if (check_free)
		ft_free(ac);
}

void	TestMultiple(int start_len, int loop, bool diff)
{
	char	*ac;
	char	*ex;
	int		len = start_len;
	char	chr = 'a';

	for (int i = 0; i < loop; i++) {
		if (diff) {
			len += i;
			chr += i;
		}
		ac = set_data(ft_malloc, len, chr);
		ex = set_data(malloc, len, chr);
		if (!ac)
			break;
		EXPECT_EQ(strncmp(ac, ex, len), 0) << "malloc should reserve its contents";
		//ft_free(ac);
		free(ex);
	}
}

/* -------------------------------------------------------------------------- */
/*                              Malloc Tiny Tests                             */
/* -------------------------------------------------------------------------- */

TEST(MallocTinyTest, One) {
	TestOne(42, false);
	TestOne(1007, false);
}

TEST(MallocTinyTest, SameMultiple) {
	TestMultiple(42, 5, false);
	TestMultiple(2, 5, false);
}

TEST(MallocTinyTest, DiffMultiple) {
	TestMultiple(75, 5, true);
	TestMultiple(21, 8, true);
	TestMultiple(100, 12, true);
}

TEST(MallocTinyTest, Small) {
	TestOne(0, false);
	TestOne(1, false);
	TestMultiple(0, 7, true);
}

TEST(MallocTinyTest, BigMultiple) {
	TestMultiple(1000, 5, true);
	TestMultiple(1000, 100, false);
	TestMultiple(1000, 200, false);
}

TEST(MallocTinyTest, OverLimit) { // does not segfault anyway
	void	*ptr[104];
	for (int i = 0; i < 104; i++)
		ptr[i] = malloc(1000);
	free(ptr[0]);
}

/* -------------------------------------------------------------------------- */
/*                             Malloc Small Tests                             */
/* -------------------------------------------------------------------------- */

TEST(MallocSmallTest, One) {
	TestOne(1008, false);
	TestOne(1009, false);
	TestOne(3200, false);
	TestOne(10403, false);
}

TEST(MallocSmallTest, SameMultiple) {
	TestMultiple(420, 5, false);
	TestMultiple(999, 19, false);
}

TEST(MallocSmallTest, DiffMultiple) {
	TestMultiple(3721, 5, true);
	TestMultiple(3059, 8, true);
}

TEST(MallocSmallTest, BigMultiple) {
	TestMultiple(1040383, 5, true);
	TestMultiple(1000000, 100, true);
}

TEST(MallocSmallTest, FillUp) {
	TestMultiple(100000, 110, true);
	TestMultiple(10000, 110, true);
	ft_malloc(8500);
}

/* -------------------------------------------------------------------------- */
/*                             Malloc Large Tests                             */
/* -------------------------------------------------------------------------- */

TEST(MallocLargeTest, One) {
	TestOne(1040384, false);
	TestOne(2040384, false);
	ft_malloc(1040384);
	ft_malloc(2040384);
}

TEST(MallocLargeTest, OneFree) {
	TestOne(1040384, false);
	TestOne(204000384, true);
}

TEST(MallocLargeTest, BigMultiple) {
	//TestMultiple(1040384, 100, true);
}

/* -------------------------------------------------------------------------- */
/*                              Malloc Free Tests                             */
/* -------------------------------------------------------------------------- */

TEST(MallocFreeTest, Zero) {
	void	*ac;

	ac = ft_malloc(0);
	ft_free(ac);
}

/* -------------------------------------------------------------------------- */
/*                                 Error Tests                                */
/* -------------------------------------------------------------------------- */

TEST(ErrorTest, Malloc) {
	ft_malloc(MALLOC_ABSOLUTE_SIZE_MAX + 1);
	ft_malloc(MALLOC_ABSOLUTE_SIZE_MAX);
	//ft_malloc(MALLOC_ABSOLUTE_SIZE_MAX - 1);
	//dlclose(handle_malloc);
	//dlclose(handle_free);
}
