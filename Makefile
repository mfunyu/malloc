# ---------------------------------------------------------------------------- #
#                                    SOURCES                                   #
# ---------------------------------------------------------------------------- #

SRCS	:= malloc.c \
			show_alloc_heap.c \
			show_free_list.c \
			free.c
#			realloc.c \
			calloc.c \
			valloc.c \
			alloc_debug.c

# ---------------------------------------------------------------------------- #
#                                     PATHS                                    #
# ---------------------------------------------------------------------------- #

DIR_OBJS:= objs
LIBFT	:= libft
PRINTF	:= ft_printf
VPATH	:= srcs

# ---------------------------------------------------------------------------- #
#                                   VARIABLES                                  #
# ---------------------------------------------------------------------------- #

NAME	= libft_malloc_$(HOSTTYPE).so
CC		:= gcc
CFLAGS	=  -Wall -Wextra -Werror
INCLUDES:= -I $(LIBFT) -I $(PRINTF) -I .
LIBS	:= -L$(LIBFT) -lft -L$(PRINTF) -lftprintf
OBJS	:= $(addprefix $(DIR_OBJS)/, $(SRCS:.c=.o))
DEPS	:= $(OBJS:.o=.d)
HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

ifeq ($(shell dpkg-architecture -qDEB_HOST_ARCH), amd64)
	CFLAGS += -fPIC
	HOST_ARCH = .amd64
else ifeq ($(shell uname), Darwin)
	DARWIN = 1
endif

# ---------------------------------------------------------------------------- #
#                                  BASIC RULES                                 #
# ---------------------------------------------------------------------------- #

.PHONY	: all
all		: $(NAME)

-include $(DEPS)

$(NAME)	: $(DIR_OBJS) $(OBJS) $(DEPS)
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

# ---------------------------------------------------------------------------- #
#                                ADVANCED RULES                                #
# ---------------------------------------------------------------------------- #

.PHONY	: setup
setup	:
	cp .env.example$(HOST_ARCH) .env
	@echo RUN source .env

.PHONY	: test
test	: all setup
	$(CC) $(CFLAGS) $(INCLUDES) ./test/test.c $(LIBS)

ifdef DARWIN
	DYLD_INSERT_LIBRARIES=./libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1 ./a.out
else
	LD_PRELOAD=./libft_malloc.so ./a.out
endif

.PHONY	: test2
test2	: all setup
	$(CC) $(INCLUDES) ./test/test2.c $(LIBS)

ifdef DARWIN
	DYLD_INSERT_LIBRARIES=./libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1 ./a.out
else
	LD_PRELOAD=./libft_malloc.so ./a.out
endif

