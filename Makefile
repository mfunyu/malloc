NAME	= libft_malloc_$(HOSTTYPE).so

SRCS	:= malloc.c \
			free.c \
			realloc.c
OBJS_DIR:= objs/
OBJS	:= $(addprefix $(OBJS_DIR), $(SRCS:.c=.o))
LIBFT	:= libft
PRINTF	:= ft_printf
VPATH	:= srcs/
INCLUDES:= -I $(LIBFT) -I $(PRINTF)

CC		:= gcc
CFLAGS	:= -Wall -Wextra -Werror $(INCLUDES)
HOSTTYPE ?= $(shell uname -m)_$(shell uname -s)

.PHONY	: all
all		: $(NAME)

$(NAME)	: $(OBJS_DIR) $(OBJS)
	$(MAKE) -C $(LIBFT)
	$(MAKE) -C $(PRINTF) LIBFT=../$(LIBFT)
	$(CC) $(CFLAGS) -shared -o $@ $(OBJS) -L$(LIBFT) -lft -L$(PRINTF) -lftprintf

$(OBJS_DIR)%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJS_DIR):
	@mkdir $@

.PHONY	: clean
clean	:
	$(MAKE) clean -C $(LIBFT)
	$(MAKE) clean -C $(PRINTF) LIBFT=../$(LIBFT)
	$(RM) -R $(OBJS_DIR)

.PHONY	: fclean
fclean	: clean
	$(MAKE) fclean -C $(LIBFT)
	$(MAKE) fclean -C $(PRINTF) LIBFT=../$(LIBFT)
	$(RM) $(NAME)

.PHONY	: re
re		: fclean all

.PHONY	: export_env
export_env	: 
	export DYLD_LIBRARY_PATH=.
	export DYLD_INSERT_LIBRARIES=myuid.so
	export DYLD_FORCE_FLAT_NAMESPACE=1
	echo $(DYLD_LIBRARY_PATH)

.PHONY	: set_env
set_env	: 
	setenv DYLD_LIBRARY_PATH .
	setenv DYLD_INSERT_LIBRARIES myuid.so
	setenv DYLD_FORCE_FLAT_NAMESPACE 1
