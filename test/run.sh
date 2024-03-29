#!/bin/bash

# define
THICK="\033[1m"
CYAN="\033[36m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET="\033[m"
PROMPT="${CYAN}${THICK}$>${RESET}"

if [ $# != 1 ] ; then
	printf "usage: ./run.sh [test number]\n"
	exit 1
fi

printf "${GREEN}[ RUN test no.$1 ]\n"
printf "${PROMPT} "
make single_test FILENAME="test$1.c"
printf "${PROMPT} "
make original FILENAME="test$1.c"

if [ $? == 0 ] ; then
	printf "${CYAN}> executable 'original' created for compareson\n${RESET}"
else
	printf "${RED}> executable 'original' cannot be created for this test\n${RESET}"
fi

if [ $(uname) == "Darwin" ] ; then
	DYLD_INSERT_LIBRARIES=./libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1 ./single_test
else
	LD_PRELOAD=./libft_malloc.so LD_LIBRARY_PATH=. ./single_test
fi


