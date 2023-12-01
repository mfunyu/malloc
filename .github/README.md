# malloc

# Tests

## `test/correction`

- basic tests

Test no. | Name | Details
:--:|:--:|:--:
0 | basic test | xmallocs -> xfrees : malloc multiple size in all size regions, and free them all at the end. Check most basic allocations.
1 | basic test1 | malloc -> free -> malloc : mallc the exact same size region just after the free of the same size block.

