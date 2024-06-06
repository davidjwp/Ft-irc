#ifndef CLIENT_HPP
# define CLIENT_HPP

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
#include "irc.hpp"

class Channel;
class Client {
private:

	std::string	_hostname;
	std::string	_nickname;
	std::string	_username;
	std::string	_realname;
	std::vector<Channel> _channels;

	int	_clfd;
	int _state;
	std::string _msg;
	string _invited_channel;
public:
	Client();
	Client& operator=(Client&);
	Client(int clientfd, const std::string hostname);
	~Client();

	//getters
	const std::string Get_host() const;
	const std::string Get_nick() const;
	std::vector<Channel>& Get_chan();
	const std::string Get_user() const;
	const std::string Get_msg() const;
	const std::string Get_realname() const;
	const string getInvitChannel() const;
	int	Get_state() const;
	int Get_clfd() const;

	//setters
	void	Set_host(std::string);
	void	Set_nick(std::string);
	void	add_chan(Channel&);
	void	Set_chan(std::string);
	void	Set_user(std::string);
	void	Set_msg(std::string);
	void	Set_realname(std::string);
	void	Set_state(int);
	void	Set_InvitedChannel(string);
	void	addMsg(std::string);

	void	isRegistered();
	const std::string makeCLname() const;
	void reply(const std::string&) const;
};
#endif