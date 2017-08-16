NAME = scop
CC = gcc
FLAGS = -Wall -Wextra
ENDFLAGS = -L libft -lft -lGL -lSDL2 -lSDL2_image -lm
INCLUDE = scop.h matrix.h vector.h bmp.h
SRC =	scop.c error.c window.c load.c load2.c\
		bmp_loader.c load_mtl.c buffer.c\
		matrix/new.c matrix/clean.c matrix/set_id.c\
		matrix/mat_x_mat.c matrix/mat_x_vec.c\
		matrix/debug.c matrix/lookat.c\
		matrix/perspective.c\
		vector/new.c vector/normalize.c vector/add.c\
		vector/sub.c vector/mult.c vector/scalar.c\
		vector/cross.c vector/debug.c
OBJ = $(SRC:%.c=src/%.o)

.PHONY: all

all: $(NAME)

%.o: %.c $(INCLUDE:%.h=include/%.h)
	@echo "\033[38;5;214mcompiling $@\033[0m"
	@$(CC) $(FLAGS) -I include -I libft/include -o $@ -c $<

libft/libft.a:
	@make -C libft

libmatrix/libmatrix.a:
	@make -C libmatrix

$(NAME): $(OBJ) libft/libft.a libmatrix/libmatrix.a
	@echo "\033[38;5;196mcompiling $@\033[0m"
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(ENDFLAGS)

clean:
	@echo "\033[38;5;6mremoving objects\033[0m"
	@rm -f $(OBJ)

fclean: clean
	@echo "\033[38;5;27mremoving $(NAME)\033[0m"
	@rm -f $(NAME)

re: fclean all
