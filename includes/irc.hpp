#pragma once
#ifndef IRC_HPP
# define IRC_HPP

// Library Includes
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include <algorithm>
#include <signal.h>
#include <vector>
#include <exception>
#include <fcntl.h>
#include <string>
#include <sstream>
#include <errno.h>
#include <map>
#include <cctype>

// File Includes
#include "reply.hpp"
#include "server.hpp"
#include "channel.hpp"
#include "client.hpp"

extern bool	stop_server;

enum Credentials{
	PASS = 1,
	NICK = 2,
	USER = 4,
	REG  = 8,
	INV	 = 16,
	OPER = 32,//not gonna use this 
};

#define USERLEN 32

#define BUFFER_SIZE 256

//ESCAPE CODES
#define Bblue "\033[1;34m"
#define BBred "\033[41m"
#define Bred "\033[1;31m"
#define Bgreen "\033[1;32m"
#define Bcyan "\033[1;36m"
#define Byellow "\033[1;33m"
#define rescol "\033[0m"
#define colstring(col, msg)(col + msg + rescol)

//EXCEPTION
class Error: public std::exception{
private:
	std::string	_msg;

public:
	virtual ~Error() throw (){};
	Error(const char* msg): _msg(std::string(msg)){};
	const char* what() const throw(){ return _msg.c_str();}
};

#endif