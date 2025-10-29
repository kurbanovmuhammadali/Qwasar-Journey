
NAME = my_tar

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC = my_tar.c tar_ops.c utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME) archive.tar

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

archive.tar:
	touch archive.tar

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) archive.tar

re: fclean all
