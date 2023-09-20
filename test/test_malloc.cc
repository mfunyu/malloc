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

TEST(MallocTest, One) {
	char	*ac;
	char	*ex;
	int		len = 42;
	char	chr = 'a';

	ac = set_data(ft_malloc, len, chr);
	ex = set_data(malloc, len, chr);
	EXPECT_EQ(strncmp(ac, ex, len), 0) << "malloc should reserve its contents";
	free(ex);
}

TEST(MallocTest, SameMultiple) {
	char	*ac;
	char	*ex;
	int		len = 42;
	char	chr = 'a';

	for (int i = 0; i < 5; i++) {
		ac = set_data(ft_malloc, len, chr);
		ex = set_data(malloc, len, chr);
		EXPECT_EQ(strncmp(ac, ex, len), 0) << "malloc should reserve its contents";
		free(ex);
	}
}

TEST(MallocTest, DiffMultiple) {
	char	*ac;
	char	*ex;
	int		len = 21;
	char	chr = '*';

	for (int i = 0; i < 7; i++) {
		len += i;
		chr += i;
		ac = set_data(ft_malloc, len, chr);
		ex = set_data(malloc, len, chr);
		EXPECT_EQ(strncmp(ac, ex, len), 0) << "malloc should reserve its contents";
		free(ex);
	}
}