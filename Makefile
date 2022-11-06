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
CFLAGS	:= -Wall -Wextra -Werror $(INCLUDES)
INCLUDES:= -I $(LIBFT) -I $(PRINTF)
OBJS	:= $(addprefix $(DIR_OBJS), $(SRCS:.c=.o))
HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

# ---------------------------------------------------------------------------- #
#                                  BASIC RULES                                 #
# ---------------------------------------------------------------------------- #

.PHONY	: all
all		: $(NAME)

$(NAME)	: $(DIR_OBJS) $(OBJS)
	$(MAKE) -C $(LIBFT)
	$(MAKE) -C $(PRINTF) LIBFT=../$(LIBFT)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJS) -L$(LIBFT) -lft -L$(PRINTF) -lftprintf
	ln -s $(NAME) libft_malloc.so

$(DIR_OBJS)%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

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
	cp .env.example .env
	sed -ie 's/example.so/$(NAME)/' .env
	@echo run source .env