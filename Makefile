NAME	= ircserv

CC		= c++

CFLAGS	= -g -std=c++98 -Wextra -Werror -Wall 

HEADERS	= Server.hpp

SRCS	= main.cpp

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
