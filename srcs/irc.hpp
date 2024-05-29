#pragma once
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
#include <string>
#include <sstream>
#include <errno.h>

extern bool	stop_server;

enum Credentials{
	PASS = 1,
	NICK = 2,
	USER = 4,
	REG  = 8,
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

class Channel {
private:

	std::string	_name;
	std::vector<std::string>	_clients;
public:

};

class Client {
private:

	std::string	_hostname;
	std::string	_nickname;
	std::string	_username;
	std::string	_realname;
	std::string	_channel;
	std::string _servname;

	int	_clfd;
	int _state;
	std::string _msg;

public:
	Client(int clientfd, const std::string hostname);
	~Client();



	//getters
	const std::string Get_host() const;
	const std::string Get_nick() const;
	const std::string Get_chan() const;
	const std::string Get_user() const;
	const std::string Get_msg() const;
	const std::string Get_realname() const;
	const std::string Get_servname() const;
	int	Get_state() const;
	int Get_clfd() const;

	//setters
	void	Set_host(std::string);
	void	Set_nick(std::string);
	void	Set_chan(std::string);
	void	Set_user(std::string);
	void	Set_msg(std::string);
	void	Set_realname(std::string);
	void	Set_state(int);
	void	Set_servname(std::string);
	void	addMsg(std::string);

	void	isRegistered();
	std::string& makeCLname() const;
	void reply(const std::string&) const;
};

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

public:
	~Server();
	Server(int port, const std::string &pass);

	int 	Start_server();
	void	Add_client();
	void	Clients_Status();
	void	Client_messages(int);
	std::string Get_message(int fd);
	void	Proc_message(std::string, int);
	std::vector<Client>::iterator	getClientit(int fd);
	std::vector<pollfd>::iterator	getPollfd(int fd);
	std::vector<std::string> split(std::string);
	void	Disconnect_client(int);


	//getters

	std::string Get_host();
	std::string Get_pass();
	std::string Get_opass();

	void	cNICK(std::vector<std::string>, int);
	void	cUSER(std::vector<std::string>, int);
	void	cPASS(std::vector<std::string>, int);
	void	cPRIVMSG(std::vector<std::string>, int);
	void	cJOIN(std::vector<std::string>, int);
	//void	cWHO(std::vector<std::string>, int);
	//void	cPING(std::vector<std::string>, int);
	void	cOPER(std::vector<std::string>, int);
	void	cPART(std::vector<std::string>, int);
	void	cNAMES(std::vector<std::string>, int);
	void	cMODE(std::vector<std::string>, int);
	void	cKICK(std::vector<std::string>, int);
	void	cINVITE(std::vector<std::string>, int);
	void	cNOTICE(std::vector<std::string>, int);
	void	cTOPIC(std::vector<std::string>, int);
	void	cKILL(std::vector<std::string>, int);
	void	cLIST(std::vector<std::string>, int);

	static void	reply(Client, std::string);
};

//NUMERIC REPLIES
class Reply{

public:
	//(001)
	static void RPL_WELCOME(const Client&);
	//(431) 
	static void ERR_NONICKNAMEGIVEN(const Client&);
	//(432)
	static void ERR_ERRONEUSNICKNAME(const Client&, const std::string&);
	//(433)
	static void ERR_NICKNAMEINUSE(const Client&, const std::string&);
	//(461)
	static void ERR_NEEDMOREPARAMS(const Client&, const std::string&);
	//(462)
	static void ERR_ALREADYREGISTERED(const Client&);
	//(464)
	static void ERR_PASSWDMISMATCH(const Client&);
};

#endif