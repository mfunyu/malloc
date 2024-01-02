# malloc

# Tests

## `test/correction`

### usage

- run 
  ```
  ./test/run.sh [N]
  ```
- compare
  - result cannot be compared with original version if this message is shown     
    >\> executable 'expected' cannot be created for this test

  - the same test can be run with original malloc by `./expected` if this message is shown
    >\> executable 'expected' created for compareson
    run
    ```
    ./expected
    ```

### basic tests

Test no. | Name | Details
:--:|:--:|:--
0 | basic test | `Nmallocs -> Nfrees` : malloc multiple size in all size regions, and free them all at the end. Check most basic allocations.
1 | basic test1 | `malloc -> free -> malloc` : malloc the exact same size region just after the free of the same size block.
2 | basic test2 | `malloc -> wirte -> read` : malloc ereas and write and read from them to varify if it works.
3 | tiny test | `Nmalloc` : malloc as much as possible with tiny allocation size.
4 | small test | `Nmalloc` : malloc as much as possible with small allocation size.
5 | large test | `Nmalloc` : malloc as much as possible with large allocation size.

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
