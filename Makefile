#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abara <banthony@student.42.fr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/12 10:49:57 by abara             #+#    #+#              #
#    Updated: 2019/12/12 10:51:09 by abara            ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = scop

OBJ_PATH = ./obj/

PATH_SRC = ./src/

PATH_HEAD = ./include/

HEADER_FILE = scop.h	\

SRC_FILE +=	main.c

SRC = $(SRC_FILE:%c=$(PATH_SRC)%c)
INCLUDE = $(HEADER_FILE:%h=$(PATH_HEAD)%h)

OBJ = $(SRC_FILE:.c=.o)
OBJ2 = $(OBJ:%.o=$(OBJ_PATH)%.o)

UNAME := $(shell uname)

LIBFT = ./libft

LIBFT_NAME = -L $(LIBFT) -lft
LIBFT_NAME_SANIT = -L $(LIBFT) -lft_sanit

ifeq ($(UNAME), Linux)
MLX_LIB = ./minilibx_linux/
HEAD_DIR = -I ./include -I $(LIBFT)
LIB =
FLAGS = -Wall -Wextra -Werror
endif

ifeq ($(UNAME), Darwin)
MLX_LIB = ./minilibx_macos/
HEAD_DIR = -I ./include -I $(LIBFT)
LIB =
FLAGS = -Wall -Wextra -Werror -Weverything
endif

DEBUG = -g3 -fsanitize=address

TRASH = Makefile~		\
		./src/*.c~		\
		./include/*.h~	\

all: $(NAME)

normal: $(SRC) $(INCLUDE)
	make -C $(LIBFT)
	gcc $(FLAGS) $(HEAD_DIR) -c $(SRC)
	mkdir -p $(OBJ_PATH)
	mv $(OBJ) $(OBJ_PATH)
	gcc $(FLAGS) $(OBJ2) $(HEAD_DIR) $(LIBFT_NAME) $(LIB)  -o $(NAME)

$(NAME): $(SRC) $(INCLUDE)
	make -C $(LIBFT) sanit
	gcc $(FLAGS) $(HEAD_DIR) -c $(SRC) $(DEBUG)
	mkdir -p $(OBJ_PATH)
	mv $(OBJ) $(OBJ_PATH)
	gcc $(FLAGS) $(OBJ2) $(HEAD_DIR) $(LIBFT_NAME_SANIT) $(LIB) -o $(NAME) $(DEBUG)

clean:
	make clean -C $(LIBFT)
	rm -rf $(OBJ_PATH) $(TRASH)

fclean: clean
	make fclean -C $(LIBFT)
	rm -f $(NAME)
	-rm $(OBJ)

re: fclean all
