NAME	= libft_malloc_$(HOSTTYPE).so

SRCS	:= malloc.c \
			free.c \
			realloc.c
OBJS_DIR:= objs/
OBJS	:= $(addprefix $(OBJS_DIR), $(SRCS:.c=.o))
DEPS	:= $(OBJS:.o=.d)
LIBFT	:= libft
VPATH	:= srcs/
INCLUDES:= -I $(LIBFT)

CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror -MMD -MP $(INCLUDES)
HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

.PHONY	: all
all		: $(NAME)

-include $(DEPS)

$(NAME)	: $(OBJS_DIR) $(OBJS)
	$(MAKE) -C $(LIBFT)
	$(CC) $(CFLAGS) -shared -fpic -o $@ $(OBJS) -L$(LIBFT) -lft

$(OBJS_DIR)%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJS_DIR):
	@mkdir $@

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
