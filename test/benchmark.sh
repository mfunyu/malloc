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
printf "${YELLOW}This test must be run with 'make bonus' compilation\n"
printf "${PROMPT} "
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
