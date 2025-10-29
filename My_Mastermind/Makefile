APP = my_mastermind
SRC_FILES = play.c codegen.c helpers.c
OBJ_FILES = $(SRC_FILES:.c=.o)
COMPILER = cc
COMPILER_FLAGS = -Wall -Wextra -Werror

all: $(APP)

$(APP): $(OBJ_FILES)
	$(COMPILER) $(COMPILER_FLAGS) $(OBJ_FILES) -o $(APP)

%.o: %.c
	$(COMPILER) $(COMPILER_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES)

fclean: clean
	rm -f $(APP)

re: fclean all

.PHONY: all clean fclean re
