# Compiler and flags
CC 			= c++
CFLAGS 		= -Wall -Wextra -Werror -std=c++98 #-fsanitize=address

#Sources and objects
SRC = main.cpp Settings.cpp ServerConfig.cpp LocationConfig.cpp
OBJ = $(SRC:.cpp=.o)
HDR = Settings.hpp ServerConfig.hpp LocationConfig.hpp

NAME = webserv

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ)

%.o: %.cpp $(HDR) Makefile
	$(CC) $(CFLAGS) -o $@ -c $<

# Clean up object files
clean:
	rm -f $(OBJ)
# Clean up object files and the library
fclean: clean
	rm -f $(NAME)
# Rebuild everything
re: fclean all
# By default, just run 'make all'
.DEFAULT_GOAL := all