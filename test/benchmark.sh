#!/bin/bash

# define
THICK="\033[1m"
CYAN="\033[1;36m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET="\033[m"
PROMPT="${CYAN}$>${RESET}"

printf "${GREEN}[ Benchmark Test ]\n"
printf "${PROMPT} "
make single_test FILENAME="test_bench.c"
if [ $? -ne 0 ] ; then
	printf "${RED}Error: make failed\n"
	exit 1
fi

if [ $(uname) == "Darwin" ] ; then
	export DYLD_INSERT_LIBRARIES=./libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1
else
	export LD_PRELOAD=./libft_malloc.so LD_LIBRARY_PATH=.
fi

time ./single_test 1
time ./single_test 10
time ./single_test 100
time ./single_test 1000
time ./single_test 10000
