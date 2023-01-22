# ---------------------------------------------------------------------------- #
#                                    SOURCES                                   #
# ---------------------------------------------------------------------------- #

SRCS	:= malloc.c \
			free.c \
			realloc.c

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
CFLAGS	= -Wall -Wextra -Werror
INCLUDES:= -I $(LIBFT) -I $(PRINTF) -I .
LIBS	:= -L$(LIBFT) -lft -L$(PRINTF) -lftprintf
OBJS	:= $(addprefix $(DIR_OBJS)/, $(SRCS:.c=.o))
HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

ifeq ($(shell dpkg-architecture -qDEB_HOST_ARCH), amd64)
	CFLAGS += -fPIC
	HOST_ARCH = .amd64
endif

# ---------------------------------------------------------------------------- #
#                                  BASIC RULES                                 #
# ---------------------------------------------------------------------------- #

.PHONY	: all
all		: $(NAME)

$(NAME)	: $(DIR_OBJS) $(OBJS)
	$(MAKE) -C $(LIBFT) CFLAGS="$(CFLAGS)"
	$(MAKE) -C $(PRINTF) LIBFT=../$(LIBFT) CFLAGS="$(CFLAGS)"
	$(CC) $(CFLAGS) -shared -o $@ $(OBJS) $(LIBS)
	ln -sf $(NAME) libft_malloc.so

$(DIR_OBJS)/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

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
	@echo run source .env

.PHONY	: test
test	: all setup
	$(CC) $(CFLAGS) $(INCLUDES) ./test/main.c $(LIBS)
	DYLD_INSERT_LIBRARIES=./libft_malloc.so DYLD_FORCE_FLAT_NAMESPACE=1 ./a.out 