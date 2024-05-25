#ifndef IRC_HPP
# define IRC_HPP

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

bool	stop_server;

enum Credentials{
	PASS = 1,
	NICK = 2,
	USER = 4,
};

//ESCAPE CODES
#define Bblue "\033[1;34m"
#define Bred "\033[1;31m"
#define Bgreen "\033[1;32m"
#define Bcyan "\033[1;36m"
#define Byellow "\033[1;33m"
#define rescol "\033[0m"
#define colstring(col, msg)(col + msg + rescol)

class Server {
private:
	std::string	_host;
	std::string _pass;
	std::string	_opass;

	int	_port;
	int	_sock;

	std::vector<pollfd> _pollfds;
	std::vector<Client> _clients;
	std::vector<Channel> _channels;

	Server();
	Server(Server&);
	Server& operator=(Server&);
public:
	~Server();
	Server(int port, const std::string &pass);

	int 	Start_server();
	void	Add_client();
	void	Client_Status();


	//getters

	std::string Get_host();
	std::string Get_pass();
	std::string Get_opass();
};

class Client {
private:

	std::string	_hostname;
	std::string	_nickname;
	std::string	_username;
	std::string	_realname;
	std::string	_channel;

	int	_clfd;
	int _state;

	Client();
	Client(Client&);
	Client& operator=(Client&);
public:
	Client(int clientfd, const std::string hostname);
	~Client();

	std::string Get_host();
	std::string Get_nick();
	std::string Get_chan();
	std::string Get_user();
	std::string	Get_realname();
	int	Get_state();
	int Get_clfd();


};

class Channel {
private:

	std::string	_name;
	std::vector<std::string>	_clients;
public:



};

#endif