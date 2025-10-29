
CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -Wno-unused-but-set-variable
NAME = my_blockchain
SRC = my_blockchain.c

all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) -o $(NAME) $(SRC)

clean:
	rm -f blockchain.bak

fclean: clean
	rm -f $(NAME)

re: fclean all


