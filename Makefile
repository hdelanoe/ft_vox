NAME		= 	ft_vox

# src / obj

SRC_PATH	=	src
SRC			=	$(shell find $(SRC_PATH) -name *.cpp)

OBJ_PATH	=	.obj
OBJ    		=	$(patsubst $(SRC_PATH)/%.cpp,$(OBJ_PATH)/%.o,$(SRC))

# librairies / includes
LIB_PATH	=	lib
GLAD		=	$(LIB_PATH)/glad
GLFW 		= 	$(LIB_PATH)/glfw
GLM			=	$(LIB_PATH)/glm
FT			=	$(LIB_PATH)/freetype
FNOISE		=	$(LIB_PATH)/fastnoise

INC_PATH	=	include
LIB			=	$(GLAD) $(GLFW) $(GLM) $(FT) $(FNOISE)
INC 		=	$(addprefix -I,$(INC_PATH))
LINK 		=	$(addprefix -L,$(LIB))
LINK_LINUX	=	$(addprefix -L,$(LIB_PATH)/linux)

#compiler
CC				= 	clang++ -std=c++11
FLAGS			= 	-Wall -Wextra -Werror -g -O2
LIBFLAG			= 	-lglad -lglfw3 -lglm_static -lfreetype -lfastnoise -framework OpenGL -framework AppKit -framework IOKit -framework Cocoa -framework CoreVideo
LIBFLAG_LINUX	= 	-ldl -lGL -lX11 -lm -lglfw -lglad -lglm -lfreetype -lfastnoise


CG = \033[92m
CY = \033[93m
CE = \033[0m

all:		$(NAME)

$(NAME):	$(OBJ)
			@ $(CC) $(FLAGS) $(LINK) $(INC) $(LIBFLAG) -o $@ $^
			@ echo "\n$(CG)---> FT_VOX program created ✓$(CE)";


linux:			$(OBJ)
			$(CC) $(FLAGS) $(LINK_LINUX) $(INC) $(LIBFLAG_LINUX) -o $@ $^
			echo "\n$(CG)---> FT_VOX program created ✓$(CE)";



$(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
			mkdir -p $(OBJ_PATH)
			@ $(CC) $(INC) -c $< -o $@
			@ echo "\033[K$(CY)[FT_VOX] :$(CE) $(CG)Compiling $<$(CE) \033[1A";

clean:

			@ /bin/rm -rf $(OBJ_PATH)
			@ echo "$(CY)---> All .o files cleared $(CG)✓$(CE)";

fclean:		clean
			@ /bin/rm -f $(NAME)
			@ echo "$(CY)---> Binary cleared $(CG)✓$(CE)";

cleanlib:
			@ /bin/rm -rf $(LIB_PATH)
			@ /bin/rm -rf $(INC_PATH)
			@ echo "$(CY)---> All lib files cleared $(CG)✓$(CE)";

re: 		fclean all

install:	cleanlib
			@ /bin/bash install.sh
			@ make re

.PHONY: 	all, clean, fclean, re, cleanlib, install
.SILENT:
