# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mbenjell <mbenjell@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/05/25 13:47:03 by mbenjell          #+#    #+#              #
#    Updated: 2019/05/08 19:15:27 by mbenjell         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = test

SRC = 	test.c \
		automate.c \
		update.c \
		code.c \
		stream.c

LIBMLX = ./minilibx/libmlx.a -framework OpenGL -framework AppKit

FLAG = -g3 -fsanitize=address #-Wall -Werror -Wextra

RM = /bin/rm -f

OBJ = $(addsuffix .o,$(basename $(SRC)))

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
		@make -C minilibx
		@gcc $(FLAG) $(SRC) -o $(NAME) $(LIBFT) $(LIBMLX)

clean:
		@$(RM) $(OBJ)
fclean:	clean
		@$(RM) $(NAME)
re:		fclean all
