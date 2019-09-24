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

# -----------------------------------------------------------
# Frameworks

# FRAMEWORKS = -framework OpenGL -framework SDL2 -framework OpenCL
FRAMEWORKS = 
FRAMEWORKSPATH = -F /Library/Frameworks

# -----------------------------------------------------------
# Libraries

# libGLEW

LIBGLEWBIN = libs/libglew/src/lib/libGLEW.a

LIBGLEWINC = libs/libglew/src/include

LIBGLEW = libs/libglew/src/lib

# libglm

LIBGLMINC = libs/libglm

# libLodePNG

LIBLODEPNGBIN = libs/liblodepng/libLodePNG.a

LIBLODEINC = libs/liblodepng

LIBLODEPNG = libs/liblodepng

# -----------------------------------------------------------
# Compiler rules

LIBS = -lGLEW -lLodePNG -lOpenGL -lSDL2 -lOpenCL -lGLEW -lGLU -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread -lGLEW -lGLU -lGL


LIBSDIR = -L$(LIBGLEW) -L$(LIBLODEPNG)

LIBSINC = -I$(LIBGLEWINC) -I$(LIBGLMINC) -I$(LIBLODEINC)

# -----------------------------------------------------------

EXEC = mod1


SRCFILENAME := main.cpp parsing.cpp mapgen.cpp sdl_gl_init.cpp \
				compileshaders.cpp cl_init.cpp ControlPoints.cpp Water.cpp \
				Emitter.cpp textures.cpp HeightMap.cpp key_bindings.cpp \
				GLItem.cpp Particle.cpp ParticleSystemData.cpp PolyMixedKernel.cpp lodepng.cpp glew.cpp

SRC_DIR = src
SRC = $(addprefix $(SRC_DIR)/, $(SRCFILENAME))

OBJ_DIR = obj

OBJ := $(addprefix $(OBJ_DIR)/, $(SRCFILENAME:.cpp=.o))

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CCFLAGS) $(MOD1INC) $(LIBSINC) $(FRAMEWORKSPATH) -o $@ -c $<


CC = clang++

# MacOS
#CCFLAGS = -Wall -Wextra -Werror -std=c++1z -fvisibility=hidden
# Linux
CCFLAGS = -Wall -Wextra -std=c++1z -fvisibility=hidden  -DGLEW_STATIC

MOD1INC = -Iinc

all: $(EXEC)
	@echo  "\x1b[32mmod1   compiled\x1b[0m"

$(EXEC): $(LIBGLEWBIN) $(LIBLODEPNGBIN) $(OBJ)
	$(CC) $(CCFLAGS) $(FRMPATH) $(FRAMEWORKS) $(LIBSDIR) $(LIBS) $(OBJ) -o $(EXEC)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(EXEC)

re: lode.fclean glew.clean fclean all

.phony: all clean fclean re

# -----------------------------------------------------------

glew.all:
	@echo  "\x1b[32mlibglew   compiles...\x1b[0m"
	@make -C libs/libglew/src glew.lib.static

glew.clean:
	@make -C libs/libglew/src distclean

lode.all:
	@echo  "\x1b[32mliblodepng   compiles...\x1b[0m"
	@make -C libs/liblodepng/ all
	@echo  "\x1b[32mmod1   compiles...\x1b[0m"

lode.clean:
	@make -C libs/liblodepng/ clean

lode.fclean:
	@make -C libs/liblodepng/ fclean

lode.re:
	@make -C libs/liblodepng/ re


$(LIBGLEWBIN): glew.all

$(LIBLODEPNGBIN): lode.all


obj/lodepng.o:libs/liblodepng/lodepng.cpp
	$(CC) $(CCFLAGS) -o $@ -c $<

obj/glew.o:libs/libglew/src/src/glew.c
	clang -DGLEW_NO_GLU -DGLEW_STATIC -I libs/libglew/src/include -o $@ -c $< 