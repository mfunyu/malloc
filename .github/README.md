# malloc

Use bonus compilation to enable all features

![ft_malloc](https://github.com/mfunyu/malloc/assets/60470877/40ea80fb-2e8a-4f91-a58b-7f26cf748f11)

## mandatory

### compilation

```
make
```
### usage
- can be run with any program compiled with libc
- for Linux
  ```
  LD_PRELOAD=./libft_malloc.so ./a.out
  ```
- for mac
  ```
  DYLD_INSERT_LIBRARIES=./libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1 ./a.out
  ```

## bonus

 - `show_alloc_mem_ex()` function is available
 - malloc debug environment variables are enabled
 - freed memory is defragmented

### compilation
  ```
  make fclean
  make bonus
  ```

### environments

- types

  Name | Details
  :--:|:--
  MallocShowHeap | show detailed heap log and freelists at the end of the execution
  MallocShowAbbr | show abbriviated heap log and freelists at the end of the execution
  MallocShowFree | show freelists at the end of the execution
  MallocDebug | log each call to each `malloc` `free` `realloc` `calloc` function
  MallocHelp | show availiable environment variables and quick explanations

- usage
  - a. export environment variable
    ```
	export MallocShowHeap=1
	```
  - b. without exporting (for Linux)
    ```
	MallocShowHeap=1 LD_PRELOAD=./libft_malloc.so LD_LIBRARY_PATH=. ./a.out
	```

# Tests

## single tests

### usage

- run
  ```
  ./test/run.sh [N]
  ```
- compare
  - result cannot be compared with original version if this message is shown
    >\> executable 'original' cannot be created for this test

  - the same test can be run with original malloc by `./original` if this message is shown
    >\> executable 'original' created for compareson
    run
    ```
    ./original
    ```

- apply enviroment varialbes to a test without using export
  - compile
    ```
    make single_test FILENO=[N]
    ```
  - run
    ```
    MallocHelp=1 LD_PRELOAD=./libft_malloc.so LD_LIBRARY_PATH=. ./single_test
    ```

### test details

Test no. | Name | Details
:--:|:--:|:--
0 | basic test | `Nmallocs -> Nfrees` : malloc multiple size in all size regions, and free them all at the end. Check most basic allocations.
1 | basic test1 | `malloc -> free -> malloc` : malloc the exact same size region just after the free of the same size block.
2 | basic test2 | `malloc -> wirte -> read` : malloc ereas and write and read from them to varify if it works.
3 | tiny test | `Nmalloc` : malloc as much as possible with tiny allocation size.
4 | small test | `Nmalloc` : malloc as much as possible with small allocation size.
5 | large test | `Nmalloc` : malloc as much as possible with large allocation size.
6 | basic realloc test | `malloc -> realloc` : realloc malloced ptr with different sizes and check there contents.

## benchmark tests

- The library needs to be compiled with `make bonus`

```
./test/benchmark.sh
```

## Google Test

- Google Test uses malloc, so replacing original malloc will cause problems to run the test

- Therefore, the test loads malloc as `ft_malloc` using `dlopen`

- To enable google testing, malloc and realloc function does not call malloc internally. It instead calls `malloc_` function to avoid calling real malloc during testing.

- run (build)
  ```
  make gtest
  ```

- for bonus compilation
  - This will execute all tests in one process, allowing the tests of extended memory regions.
  ```
  make test
  ```
