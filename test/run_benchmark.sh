#!/bin/bash

make correction FILENAME="test_bench.c"

if [ $(uname) == "Darwin" ] ; then
	export DYLD_INSERT_LIBRARIES=./libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1
else
	export LD_PRELOAD=./libft_malloc.so LD_LIBRARY_PATH=.
fi

time ./correction 1
time ./correction 10
time ./correction 100
time ./correction 1000
time ./correction 10000
