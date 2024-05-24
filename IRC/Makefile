NAME = ircserv

CC = c++

FLAGS = -g -Werror -Wall -Wextra -std=c++98

SOURCES = ./logic/commands/channel_commands/*.cpp \
		  ./logic/commands/server_commands/*.cpp \
		  ./logic/classes.cpp \
		  ./logic/main.cpp \
		  ./logic/utils.cpp

DSYM = ./ircserv.dSYM

$(NAME):
		$(CC) $(FLAGS) $(SOURCES) -o $(NAME)

all: $(NAME)

clean:

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(DSYM)

re: fclean all