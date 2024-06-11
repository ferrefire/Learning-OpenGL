CFLAGS = -std=c++17 -O2 -g
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
SRC = src/*.cpp src/*.c
NAME = Window

$(NAME): $(SRC)
	g++ $(CFLAGS) -o $(NAME) $(SRC) $(LDFLAGS)

.PHONY: run clean re

run: $(NAME)
	./$(NAME)

clean:
	rm -f $(NAME)

re: clean $(NAME)