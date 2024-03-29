# ---------------------------------------------------------------------------- #
#                                    SOURCES                                   #
# ---------------------------------------------------------------------------- #

# srcs/
SRCS	:= malloc.c \
			realloc.c \
			calloc.c \
			free.c \
			init.c \
			allocate.c \
			show_alloc_mem.c

# srcs/utils
SRCS	+= alignment.c \
			get_page_size.c \
			mmap_by_size.c \
			is_allocated.c \
			error.c \
			extend_region.c \
			remaindering.c \
			freelist.c

# ---------------------------------------------------------------------------- #
#                                     PATHS                                    #
# ---------------------------------------------------------------------------- #

DIR_OBJS:= objs
LIBFT	:= libft
PRINTF	:= ft_printf
VPATH	:= srcs srcs/utils

# ---------------------------------------------------------------------------- #
#                                   VARIABLES                                  #
# ---------------------------------------------------------------------------- #

NAME	= libft_malloc_$(HOSTTYPE).so
CC		:= gcc
CFLAGS	= -Wall -Wextra -Werror -D FD=2
INCLUDES:= -I includes -I $(LIBFT) -I $(PRINTF) -I .
LIBS	:= -L$(LIBFT) -lft -L$(PRINTF) -lftprintf

OBJS	= $(addprefix $(DIR_OBJS)/, $(SRCS:.c=.o))
DEPS	= $(OBJS:.o=.d)
HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

ifeq ($(shell dpkg-architecture -qDEB_HOST_ARCH), amd64)
	CFLAGS += -fPIC
endif

# ---------------------------------------------------------------------------- #
#                                     BONUS                                    #
# ---------------------------------------------------------------------------- #

ifdef BONUS
	SRCS	+= print_format.c \
				set_flag.c \
				show_alloc_mem_ex.c \
				print_alloc.c \
				show_freelist.c \
				consolidation.c \
				debug.c
	VPATH	+= srcs/bonus
	CFLAGS	+= -D BONUS
	LIBS	+= -ldl
endif

ifdef DEBUG
	CFLAGS	+= -g
endif

# ---------------------------------------------------------------------------- #
#                                  BASIC RULES                                 #
# ---------------------------------------------------------------------------- #

.PHONY	: all
all	: $(NAME) ## Run basic compilation

-include $(DEPS)

$(NAME)	: $(DIR_OBJS) $(OBJS)
	$(MAKE) -C $(LIBFT) CFLAGS="$(CFLAGS)"
	$(MAKE) -C $(PRINTF) LIBFT=../$(LIBFT) CFLAGS="$(CFLAGS)"
	$(CC) $(CFLAGS) -shared -o $@ $(OBJS) $(LIBS)
	ln -sf $(NAME) libft_malloc.so

$(DIR_OBJS)/%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP $(INCLUDES) -o $@ -c $<

$(DIR_OBJS):
	@mkdir $@

.PHONY	: clean
clean	: ## Delete executables
	$(MAKE) clean -C $(LIBFT)
	$(MAKE) clean -C $(PRINTF) LIBFT=../$(LIBFT)
	$(RM) -R $(DIR_OBJS)

.PHONY	: fclean
fclean	: clean g_clean t_clean c_clean ## Delete all executables, object files, and test executables
	$(MAKE) fclean -C $(LIBFT)
	$(MAKE) fclean -C $(PRINTF) LIBFT=../$(LIBFT)
	$(RM) $(NAME) libft_malloc.so .env

.PHONY	: re
re	: fclean ## Run fclean and all
	$(MAKE) all

.PHONY	: bonus
bonus	: ## (bonus) Run bonus compilation, make fclean required for the first time
	make BONUS=1

.PHONY	: debug
debug	: ## (debug) Compile with debug flag -g, make fclean required for the first time
	make DEBUG=1

# ---------------------------------------------------------------------------- #
#                                ADVANCED RULES                                #
# ---------------------------------------------------------------------------- #

TESTDIR = ./test

FILENO = 0
FILENAME = test$(FILENO).c
.PHONY	: single_test
single_test	: all
	$(CC) $(INCLUDES) $(TESTDIR)/single_test/$(FILENAME) -L. -lft_malloc $(LIBS) -o $@

.PHONY	: original
original	: all
	$(CC) $(INCLUDES) $(TESTDIR)/single_test/$(FILENAME) $(LIBS) -o $@

.PHONY	: t_clean
t_clean	:
	$(RM) single_test original

# ---------------------------------------------------------------------------- #
#                                  GOOGLE TEST                                 #
# ---------------------------------------------------------------------------- #

GTESTDIR= $(TESTDIR)/googletest

.PHONY	: gtest
gtest	: all g_build ## (test) Build and run google tests
	cd $(GTESTDIR)/build && ctest

.PHONY	: test
test	: all g_build ## (bonus) Run google tests for bonus
	cd $(GTESTDIR)/build && MallocShowHeap=1 ./test_malloc 2> ../log

.PHONY	: g_build
g_build	: $(GTESTDIR)/build
	cd $(GTESTDIR) && cmake --build build 1> /dev/null

$(GTESTDIR)/build:
	cd $(GTESTDIR) && cmake -S . -B build

.PHONY	: g_clean
g_clean	:
	$(RM) -R $(GTESTDIR)/build
	$(RM) $(GTESTDIR)/log

# ---------------------------------------------------------------------------- #
#                                      DEV                                     #
# ---------------------------------------------------------------------------- #

CHECKDIR = $(TESTDIR)/check

.PHONY	: check 
check	:  ## (test) Compile files under ./test/check directory
	gcc -o $(CHECKDIR)/c_free $(CHECKDIR)/c_free.c
	gcc -o $(CHECKDIR)/c_malloc $(CHECKDIR)/c_malloc.c
	gcc -o $(CHECKDIR)/c_realloc $(CHECKDIR)/c_realloc.c

.PHONY	: c_clean
c_clean	:
	$(RM) $(CHECKDIR)/c_free
	$(RM) $(CHECKDIR)/c_malloc
	$(RM) $(CHECKDIR)/c_realloc

# ---------------------------------------------------------------------------- #
#                                 WORKDIR SETUP                                #
# ---------------------------------------------------------------------------- #

.PHONY	: setup
setup	: ## (dev) Setup pre-commit hook 
	cp .github/hooks/pre-commit .git/hooks/

# ---------------------------------------------------------------------------- #
#                                     HELP                                     #
# ---------------------------------------------------------------------------- #

.PHONY	: help
help	: ## Display this help screen
	@grep -E '^[a-zA-Z_-]+.*:.*?## .*$$' Makefile \
	| awk 'BEGIN {FS = "\t:.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'
