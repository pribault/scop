# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pribault <pribault@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/06/27 13:38:44 by pribault          #+#    #+#              #
#    Updated: 2018/06/29 13:51:40 by pribault         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = scop
CC = gcc
FLAGS = -Wall -Wextra -Werror -Ofast -g -fsanitize=address
ARCH =	$(shell uname -s)
ifeq ($(ARCH), Darwin)
ENDFLAGS = -L libft -lft -framework OpenGL -lSDL2 -lSDL2_image -lm
else
ifeq ($(ARCH), Linux)
ENDFLAGS = -L libft -lft -lGL -lSDL2 -lSDL2_image -lm
endif
endif
INCLUDE = scop.h matrix.h vector.h bmp.h
SRC =	scop.c error.c window.c load.c load2.c events_2.c\
		bmp_loader.c load_mtl.c buffer.c events.c load_mtl_2.c\
		matrix/new.c matrix/clean.c matrix/set_id.c\
		matrix/mat_x_mat.c matrix/mat_x_vec.c\
		matrix/debug.c matrix/lookat.c\
		matrix/perspective.c\
		vector/new.c vector/normalize.c vector/add.c\
		vector/sub.c vector/mult.c vector/scalar.c\
		vector/cross.c vector/debug.c vector/rotate.c\
		vector/quaternion.c vector/mult2.c
OBJ = $(sort $(SRC:%.c=src/%.o))
N = 0
JOBS = 1

.PHONY: all clean fclean re libft

all: libft
	@make $(NAME) -j $(JOBS)

%.o: %.c $(INCLUDE:%.h=include/%.h) Makefile
	@echo "\033[38;5;214mcompiling $@\033[0m"
	@$(CC) $(FLAGS) -I include -I libft/include -o $@ -c $<
	@$(eval N=$(shell echo $$(($(N)+1))))

libft:
	@make -C libft

libft/libft.a:
	@make -C libft

$(NAME): libft/libft.a $(OBJ)
	@echo "\033[38;5;196mcompiling $@\033[0m"
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ) $(ENDFLAGS)

clean:
	@make -C libft clean
	@echo "\033[38;5;6mremoving objects\033[0m"
	@rm -f $(OBJ)

fclean: clean
	@make -C libft fclean
	@echo "\033[38;5;27mremoving $(NAME)\033[0m"
	@rm -f $(NAME)

re: fclean all
