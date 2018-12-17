# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vpopovyc <vpopovyc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/12/09 17:42:51 by vpopovyc          #+#    #+#              #
#    Updated: 2018/12/17 16:55:27 by vpopovyc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

EXEC = mod1

SRCFILENAME := main.cpp parsing.cpp mapgen.cpp

SRC = $(addprefix core/, $(SRCFILENAME))

OBJ = $(SRC:.cpp=.o)

CC = clang++

CCFLAGS = -Wall -Wextra -Werror -std=c++1z -fvisibility=hidden

MOD1INC = -I. -Iglm-0.9.9.3

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CCFLAGS) $(OBJ) -o $(EXEC)

%.o:%.cpp
	$(CC) $(CCFLAGS) $(MOD1INC) -o $@ -c $<

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(EXEC)

re: fclean all

.phony: all clean fclean re