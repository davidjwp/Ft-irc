NAME	= ircserv

CC	= g++

CFLAGS	= -g -std=c++98 -Wextra -Werror -Wall 

HEADERS	= srcs/irc.hpp

SRCS	= srcs/main.cpp srcs/cNICK.cpp

OBJS	= $(SRCS:.cpp=.o)

all:$(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CC) -c $(CFLAGS) $< -o $@
	
clean:
	rm -f $(OBJS)

fclean: clean
	rm $(NAME)

re: fclean all
