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


char *set_data(size_t size, int chr)
{
	char *ptr = (char *)ft_malloc(size + 1);
	memset(ptr, chr, size);
	ptr[size] = '\0';

	return ptr;
}

TEST(MallocFreeTest, Simple) {
  char *ptr = set_data(30, 'a');
  ft_free(ptr);
}