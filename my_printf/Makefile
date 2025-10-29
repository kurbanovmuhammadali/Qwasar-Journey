CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g3 -fsanitize=address
NAME    = my_printf

SRC     = main.c my_printf.c
OBJ     = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
 $(CC) $(CFLAGS) -o $(NAME) $(OBJ)

clean:
 rm -f *.o

fclean: clean
 rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re