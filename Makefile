# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lvasseur <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/02/22 13:30:54 by lvasseur          #+#    #+#              #
#    Updated: 2018/11/30 16:18:58 by lvasseur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.PHONY: make all clean fclean re

NAME = ft_vox
CC = clang++

SRC_PATH = ./src/
OBJ_PATH = ./obj/
LIB_PATH = ./lib/
INC_PATH = ./include/

GCC_FLGS = #-Wall -Wextra -Werror
GCC_LIBS = -lglfw -lGLEW -lGL -lGLU -lX11 -lXxf86vm -lXrandr -lXi -ldl -lpthread -lXinerama -lXcursor -lm

SRC_NAME = main.cpp ChunkHandler.cpp Chunk.cpp Player.cpp Window.cpp shaders.cpp Texture.cpp MapGeneration.cpp FrustumCulling.cpp
OBJ_NAME = $(SRC_NAME:.cpp=.o)
LIB_NAME =

SRC = $(addprefix $(SRC_PATH), $(SRC_NAME))
OBJ = $(addprefix $(OBJ_PATH), $(OBJ_NAME))
INC = $(addprefix -I,$(INC_PATH))
LIB = $(addprefix -L$(LIB_PATH),$(LIB_NAME))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(GCC_FLGS) $(LIB) $(INC) $(OBJ) $(GCC_LIBS) -o $(NAME)

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	mkdir -p $(OBJ_PATH)
	$(CC) $(GCC_FLGS) $(INC) -o $@ -c $<

clean:
	rm -fv $(OBJ)
	rm -rf $(OBJ_PATH)

fclean: clean
	rm -fv $(NAME)

re: fclean all
