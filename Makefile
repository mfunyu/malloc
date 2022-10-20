NAME	:= libft_malloc

SRCS	:= malloc.c \
			free.c \
			realloc.c
OBJS_DIR:= objs/
OBJS	:= $(addprefix $(OBJS_DIR), $(SRCS:.cpp=.o))
DEPS	:= $(OBJS:.o=.d)
LIBFT	:= libft
VPATH	:= srcs/

CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror -MMD -MP -L$(LIBFT) -lft

.PHONY	: all
all		: $(NAME)

-include $(DEPS)

$(NAME)	: set_env $(OBJS)
	$(MAKE) -C $(LIBFT)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJS_DIR)%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJS_DIR):
	@mkdir $@

.PHONY	: set_env
set_env	:
	ifeq ($(HOSTTYPE),)
		HOSTTYPE := $(shell uname -m)_$(shell uname -s)
	endif

.PHONY	: clean
clean	:
	$(MAKE) clean -C $(LIBFT)
	$(RM) -R $(OBJS_DIR)

.PHONY	: fclean
fclean	: clean
	$(MAKE) fclean -C $(LIBFT)
	$(RM) $(NAME)

.PHONY	: re
re		: fclean all
