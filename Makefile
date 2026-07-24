CXX=c++
CXXFLAGS = -Wall -Werror -Wextra -g -std=c++11
LDFLAGS = -lglfw -lGLEW -lGL -lm -lX11 -lpthread -lXi -lXrandr
NAME = scop
SRC_DIR = ./srcs/
SRCS = main.cpp \
	   $(SRC_DIR)Obj.cpp \
	   $(SRC_DIR)GPU.cpp \
	   $(SRC_DIR)Mtl.cpp \
	   $(SRC_DIR)Matrix.cpp \

OBJ_DIR = .build/
OBJS = $(SRCS:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)  $(LDFLAGS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -c -o $@

fclean: clean
	rm -f $(NAME) $(NAME_BONUS)

clean:
	rm -rf $(OBJ_DIR) $(OBJ_DIRBONUS)

re: fclean all

.PHONY: all fclean clean re
