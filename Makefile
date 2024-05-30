NAME	= ircserv
#CHANGE TO c++`
CC	= g++

CFLAGS	= -g -std=c++98 -Wextra -Werror -Wall 

HEADERS	= srcs/irc.hpp

SRCS	= srcs/main.cpp srcs/cNICK.cpp \
						srcs/cUSER.cpp \
						srcs/cPASS.cpp \
						srcs/cMODE.cpp \
						srcs/cJOIN.cpp 

OBJS_DIR	= objs/
OBJS	= $(SRCS:%.cpp=$(OBJS_DIR)%.o)

all:$(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)%.o: %.cpp $(HEADER)
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@
	
clean:
	rm -f $(OBJS)

fclean: clean
	rm $(NAME)

re: fclean all
