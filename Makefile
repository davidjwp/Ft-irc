NAME    = ircserv

# Compiler and flags
CC      = g++
CFLAGS  = -g -std=c++98 -Wextra -Werror -Wall

# Directories and files
HEADERS = srcs/irc.hpp
SRCS    = srcs/Numerics.cpp \
		srcs/main.cpp \
		srcs/cNICK.cpp \
		srcs/cUSER.cpp \
		srcs/cPASS.cpp \
		srcs/cMODE.cpp \
		srcs/cJOIN.cpp \
		srcs/cPRIVMSG.cpp \

OBJS_DIR = objs/
OBJS    = $(SRCS:srcs/%.cpp=$(OBJS_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)%.o: srcs/%.cpp $(HEADERS)
	@mkdir -p $(OBJS_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
