CXX = g++
#CPPFLAGS = 
CXXFLAGS = -std=c++17 -O2 -g -Iincludes/
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

SRC = $(wildcard src/*.cpp)
EXTSRC = src/glad.c
OBJDIR = obj/
OBJ = $(addprefix $(OBJDIR), $(SRC:.cpp=.o)) $(addprefix $(OBJDIR), $(EXTSRC:.c=.o))
EXTOBJ = $(addprefix $(OBJDIR), $(EXTSRC:.c=.o))
DEPS = includes/*.hpp includes/*.h
NAME = Window

.PHONY: run clean fclean re

$(NAME): $(OBJ) $(DEPS)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

$(OBJDIR)%.o: %.c*
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $^ -o $@

run: $(NAME)
	./$(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)