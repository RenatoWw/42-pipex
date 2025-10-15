NAME = pipex
SRCS = pipex.c
OBJS = $(SRCS:.c=.o)
LIBFT = libft/
LIBFT_A = libft/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT_A)
	cc -Wall -Werror -Wextra $(SRCS) $(LIBFT_A) -o $(NAME)

$(LIBFT_A):
	$(MAKE) -s -C $(LIBFT)

clean:
	$(MAKE) -C $(LIBFT) clean
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

.PHONY: all clean fclean re