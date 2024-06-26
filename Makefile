NAME    = ircserv

# Compiler and flags
CXX      = c++
CXXFLAGS  = -g3 -std=c++98 -Wextra -Werror -Wall


# Directories
OBJS_DIR = objs/
INCLUDES_DIR = includes/
SRCS_DIR = srcs/

# Files
INCLUDES = channel.hpp \
			client.hpp \
			irc.hpp \
			reply.hpp \
			server.hpp \

SRCS	= main.cpp \
		channel.cpp \
		client.cpp \
		reply.cpp \
		server.cpp \
		cNICK.cpp \
		cUSER.cpp \
		cPASS.cpp \
		cMODE.cpp \
		cJOIN.cpp \
		cPRIVMSG.cpp \
		cPING.cpp \
		cOPER.cpp \
		cPART.cpp \
		cKICK.cpp \
		cINVITE.cpp \
		cTOPIC.cpp \
		cQUIT.cpp 

OBJS	= $(SRCS:%.cpp=$(OBJS_DIR)%.o)

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS) $(addprefix $(INCLUDES_DIR), $(INCLUDES))
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

$(OBJS): $(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp $(addprefix $(INCLUDES_DIR), $(INCLUDES))
	$(CXX) -c $(CXXFLAGS) -I$(INCLUDES_DIR) $< -o $@

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

clean:
	rm -rf $(OBJS) $(OBJS_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re 