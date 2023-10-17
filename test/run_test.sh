#!/bin/bash

echo run $1

make correction FILENAME="test$1.c"

if [ $(uname) == "Darwin" ] ; then
	DYLD_INSERT_LIBRARIES=./libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1 ./correction
else
	LD_PRELOAD=./libft_malloc.so LD_LIBRARY_PATH=. ./correction
fi
