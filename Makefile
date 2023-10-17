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
			lst_malloc_chunk.c \
			lst_mmap_chunk.c \
			mmap_by_size.c \
			split_chunk.c

# ---------------------------------------------------------------------------- #
#                                     PATHS                                    #
# ---------------------------------------------------------------------------- #

DIR_OBJS:= objs
LIBFT	:= libft
PRINTF	:= ft_printf
VPATH	:= srcs srcs/utils srcs/lists

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

BONUS=1
# ---------------------------------------------------------------------------- #
#                                     BONUS                                    #
# ---------------------------------------------------------------------------- #

ifdef BONUS
	SRCS	+= print.c \
				set_flag.c \
				show_alloc_heap.c \
				show_freelist.c \
				extend_region.c
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
fclean	: clean
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

FILENAME = test.c
.PHONY	: correction
correction	: all
	$(CC) $(INCLUDES) ./test/correction/$(FILENAME) $(LIBS) $(NAME) -o $@

.PHONY	: test2
test2	: all
	$(CC) $(INCLUDES) ./test/test2.c $(LIBS)

ifdef DARWIN
	DYLD_INSERT_LIBRARIES=./libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1 ./a.out
else
	LD_PRELOAD=./libft_malloc.so ./a.out
endif

.PHONY	: normal
normal	: all
	$(CC) $(INCLUDES) ./test/test2.c $(LIBS)
	./a.out

.PHONY	: gtest
gtest	: all
	cd test && cmake -S . -B build
	cd test && cmake --build build 1> /dev/null
	cd test/build && ctest

.PHONY	: test
test	:
	cd test && cmake --build build 1> /dev/null
	cd test/build && MallocShowHeap=1 ./test_malloc 2> ../log
