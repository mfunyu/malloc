# ---------------------------------------------------------------------------- #
#                                    SOURCES                                   #
# ---------------------------------------------------------------------------- #

SRCS	:= malloc.c \
			realloc.c \
			free.c \
			init.c \
			allocate.c \
			show_alloc_mem.c

SRCS	+= alignment.c \
			get_page_size.c \
			mmap_by_size.c \
			error.c \
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
CFLAGS	= -Wall -Wextra -Werror -D DEBUG
INCLUDES:= -I includes -I $(LIBFT) -I $(PRINTF) -I .
LIBS	:= -L$(LIBFT) -lft -L$(PRINTF) -lftprintf

OBJS	= $(addprefix $(DIR_OBJS)/, $(SRCS:.c=.o))
DEPS	= $(OBJS:.o=.d)
HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

ifeq ($(shell dpkg-architecture -qDEB_HOST_ARCH), amd64)
	CFLAGS += -fPIC
	HOST_ARCH = .amd64
else ifeq ($(shell uname), Darwin)
	DARWIN = 1
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
				extend_region.c \
				consolidation.c
	VPATH	+= srcs/bonus
	CFLAGS	+= -D BONUS
endif

# ---------------------------------------------------------------------------- #
#                                  BASIC RULES                                 #
# ---------------------------------------------------------------------------- #

.PHONY	: all
all		: $(NAME)

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
clean	:
	$(MAKE) clean -C $(LIBFT)
	$(MAKE) clean -C $(PRINTF) LIBFT=../$(LIBFT)
	$(RM) -R $(DIR_OBJS)

.PHONY	: fclean
fclean	: clean t_clean c_clean
	$(MAKE) fclean -C $(LIBFT)
	$(MAKE) fclean -C $(PRINTF) LIBFT=../$(LIBFT)
	$(RM) $(NAME) libft_malloc.so .env

.PHONY	: re
re		: fclean all

.PHONY	: bonus
bonus	:
	make BONUS=1

# ---------------------------------------------------------------------------- #
#                                ADVANCED RULES                                #
# ---------------------------------------------------------------------------- #
TESTDIR = ./test

FILENAME = test0.c
.PHONY	: correction
correction	: all
	$(CC) $(INCLUDES) $(TESTDIR)/correction/$(FILENAME) $(LIBS) $(NAME) -o $@

.PHONY	: original
original	: all
	$(CC) $(INCLUDES) $(TESTDIR)/correction/$(FILENAME) $(LIBS) -o $@

.PHONY	: t_clean
t_clean	:
	$(RM) correction original

# ---------------------------------------------------------------------------- #
#                                  GOOGLE TEST                                 #
# ---------------------------------------------------------------------------- #

GTESTDIR= $(TESTDIR)/googletest

.PHONY	: gtest
gtest	: all
	cd $(GTESTDIR) && cmake -S . -B build
	cd $(GTESTDIR) && cmake --build build 1> /dev/null
	cd $(GTESTDIR)/build && ctest

.PHONY	: test
test	:
	cd $(GTESTDIR) && cmake --build build 1> /dev/null
	cd $(GTESTDIR)/build && MallocShowHeap=1 ./test_malloc 2> ../log

# ---------------------------------------------------------------------------- #
#                                      DEV                                     #
# ---------------------------------------------------------------------------- #

CHECKDIR = $(TESTDIR)/check

.PHONY	: check 
check	: all
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

.PHONY	: SETUP
setup	:
	cp .github/hooks/pre-commit .git/hooks/
