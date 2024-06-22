CC = g++
CFLAGS = -std=c++17 -O2 -g -Iincludes/
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

SRC = \
	src/main.cpp \
	src/debug.cpp \
	src/errors.cpp \
	src/image_loader.cpp \
	src/mesh.cpp \
	src/rendering.cpp \
	src/shader.cpp \
	src/shaders.cpp \
	src/shape.cpp \
	src/texture.cpp \
	src/triangle.cpp \
	src/utilities.cpp \

EXTSRC = \
	src/glad.c \

OBJDIR = obj/
OBJ = $(addprefix $(OBJDIR), $(SRC:.cpp=.o))
EXTOBJ = $(addprefix $(OBJDIR), $(EXTSRC:.c=.o))
DEPS = includes/*.hpp includes/*.h
NAME = Window

.PHONY: run clean fclean re

$(NAME): $(OBJ) $(EXTOBJ) $(DEPS)
	$(CC) $(CFLAGS) $(OBJ) $(EXTOBJ) -o $(NAME) $(LDFLAGS)

$(OBJDIR)%.o: %.cpp
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(NAME)
	./$(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)