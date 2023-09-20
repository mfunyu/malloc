#include <gtest/gtest.h>
#include <stdio.h>
#include <dlfcn.h>

void* ft_malloc(size_t size)
{	
	void		*handle;
	static void	*(*my_malloc)(size_t);

	if (!my_malloc)
	{
		handle = dlopen("../../libft_malloc.so", RTLD_LOCAL | RTLD_LAZY);
		if (!handle) {
			exit(EXIT_FAILURE);
		}
		*(void **) (&my_malloc) = dlsym(handle, "malloc");
		printf("===== malloc loaded =====\n");
		//dlclose(handle);
	}
	return my_malloc(size);
}

void ft_free(void *ptr)
{	
	void		*handle;
	static void	(*my_free)(void *);

	if (!my_free)
	{
		handle = dlopen("../../libft_malloc.so", RTLD_LOCAL | RTLD_LAZY);
		if (!handle) {
			exit(EXIT_FAILURE);
		}
		*(void **) (&my_free) = dlsym(handle, "free");
		printf("===== free loaded =====\n");
		//dlclose(handle);
	}
	return my_free(ptr);
}


char *set_data(void *(*func)(size_t), size_t size, int chr)
{
	char *ptr;
	
	ptr = (char *)func(size + 1);
	memset(ptr, chr, size);
	ptr[size] = '\0';

	return ptr;
}

void	TestOne(int len)
{
	char	*ac;
	char	*ex;
	char	chr = 'a';

	ac = set_data(ft_malloc, len, chr);
	ex = set_data(malloc, len, chr);
	EXPECT_EQ(strncmp(ac, ex, len), 0) << "malloc should reserve its contents";
	free(ex);
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
		EXPECT_EQ(strncmp(ac, ex, len), 0) << "malloc should reserve its contents";
		free(ex);
	}
}

/* -------------------------------------------------------------------------- */
/*                              Malloc Tiny Tests                             */
/* -------------------------------------------------------------------------- */

TEST(MallocTinyTest, One) {
	TestOne(42);
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
	TestOne(0);
	TestOne(1);
	TestMultiple(0, 7, true);
}

/* -------------------------------------------------------------------------- */
/*                             Malloc Small Tests                             */
/* -------------------------------------------------------------------------- */

TEST(MallocSmallTest, One) {
	TestOne(3200);
	TestOne(10403);
}

TEST(MallocSmallTest, SameMultiple) {
	TestMultiple(420, 5, false);
	TestMultiple(999, 19, false);
}

TEST(MallocSmallTest, DiffMultiple) {
	TestMultiple(3721, 5, true);
	TestMultiple(3059, 8, true);
	TestMultiple(1040383, 30, true);
}

TEST(MallocSmallTest, Small) {
	TestOne(1008);
}

/* -------------------------------------------------------------------------- */
/*                              Mallo Free Tests                              */
/* -------------------------------------------------------------------------- */

TEST(MallocFreeTest, Zero) {
	void	*ac;

	ac = ft_malloc(0);
	ft_free(ac);
}