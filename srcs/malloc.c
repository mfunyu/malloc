#include "libft.h"
#include "ft_printf.h"
#include <sys/mman.h>
# include <unistd.h>
# include <errno.h>
# include <string.h>

void error_exit()
{
	char *err;
	err = strerror(errno);
	ft_putendl_fd(err, STDERR_FILENO);
	exit(1);
}

char *mem_init()
{
	int map_size;
	static char *mem_heap;

	map_size = getpagesize() * 42;
	ft_printf("map_size: %d\n", map_size);

	mem_heap = (char *)mmap(0, map_size, PROT_READ | PROT_WRITE,
							MAP_ANON | MAP_PRIVATE, -1, 0);
	if (mem_heap == MAP_FAILED) {
		error_exit();
	}
	return mem_heap;
}

char	*find_free(char *mem_heap, size_t size)
{
	static char *mem_end;
	char *mem_head;

	if (!mem_end)
		mem_end = mem_heap;
	mem_head = mem_end;
	mem_end += size;
	return mem_head;
}

char	*func(size_t size)
{
	static char *mem_heap;
	if (!mem_heap)
		mem_heap = mem_init();
	return find_free(mem_heap, size);
}

void *malloc(size_t size)
{
	ft_printf("malloc: size = %d\n", (int)size);
	return func(size);
}
