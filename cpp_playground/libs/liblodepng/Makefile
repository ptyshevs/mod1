# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vpopovyc <vpopovyc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/09/15 17:08:37 by vpopovyc          #+#    #+#              #
#    Updated: 2017/09/16 16:46:52 by vpopovyc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libLodePNG.a

SRC = lodepng.c

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror -O3

all: $(NAME)

$(NAME): $(OBJ)
	@ar rc $@ $^
	@ranlib $@
	@echo  "\x1b[32mlibLodePNG   compiled\x1b[0m"

%.o: %.c
	clang $(CFLAGS) -o $@ -c $<

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all