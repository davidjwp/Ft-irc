#ifndef REPLY_HPP
# define REPLY_HPP

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
# include "irc.hpp"

class Client;
class Channel;

class Reply {
public:
	//(001)
	static void RPL_WELCOME(const Client&);
	//(221)
	static void RPL_UMODEIS(const Client&);
	//(324)
	static void RPL_CHANNELMODEIS(const Client&, const Channel&);
	//(332)
	static void RPL_TOPIC(const Client&, Channel&);
	//(341)
	static void RPL_INVITING(const Client&, const std::string , Channel&);
	//(353)
	static void RPL_NAMREPLY(const Client&, Channel&);
	//(366)
	static void RPL_ENDOFNAMES(const Client&, std::string);
	//(381)
	static void RPL_YOUREOPER(const Client&);
	//(401)
	static void ERR_NOSUCHNICK(const Client&, const std::string&);
	//(402)
	static void	ERR_NOSUCHSERVER(const Client&, const std::string&);
	//(403)
	static void ERR_NOSUCHCHANNEL(const Client&, const std::string&);
	//(431) 
	static void ERR_NONICKNAMEGIVEN(const Client&);
	//(432)
	static void ERR_ERRONEUSNICKNAME(const Client&, const std::string&);
	//(433)
	static void ERR_NICKNAMEINUSE(const Client&, const std::string&);
	//(441)
	static void ERR_USERNOTINCHANNEL(const Client&, const std::string, const Channel&);
	//(442)
	static void ERR_NOTONCHANNEL(const Client&, const std::string);
	//(443)
	static void ERR_USERONCHANNEL(const Client&, const std::string, Channel&);
	//(451)
	static void ERR_NOTREGISTERED(const Client&);
	//(461)
	static void ERR_NEEDMOREPARAMS(const Client&, const std::string&);
	//(462)
	static void ERR_ALREADYREGISTERED(const Client&);
	//(464)
	static void ERR_PASSWDMISMATCH(const Client&);
	//(471)
	static void ERR_CHANNELISFULL(const Client&, std::string);
	//(473)
	static void ERR_INVITEONLYCHAN(const Client&, std::string);
	//(474)
	static void ERR_BANNEDFROMCHAN(const Client&, std::string);
	//(475)
	static void ERR_BADCHANNELKEY(const Client&, std::string);
	//(476)
	static void ERR_BADCHANMASK(const Client&, const std::string&);
	//(482)
	static void ERR_CHANOPRIVSNEEDED(const Client&, const std::string);
	//(501)
	static void ERR_UMODEUNKNOWNFLAG(const Client&);
	//(502)
	static void ERR_USERSDONTMATCH(const Client&);
	//(525)
	static void ERR_INVALIDKEY(const Client&, const Channel&);
};

#endif