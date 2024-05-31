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
#include <map>
#include <cctype>

extern bool	stop_server;

enum Credentials{
	PASS = 1,
	NICK = 2,
	USER = 4,
	REG  = 8,
	INV	 = 16,
	OPER = 32,
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

class Client;

class Channel {
private:
	std::string	_name;


	int	_limit;
	std::string _pass;
	std::string _topic;
	bool 	_invit_mode;
	bool	_topic_mode;
	bool	_key_mode;
	std::map<const std::string, Client> _clients;
	std::map<const std::string, int>	_operators;
	std::vector<std::string> _banned;
public:
	Channel();
	Channel(const std::string& chanName, Client client);

	const std::string getName() const;
	bool getInvit() const;
	int getLimit() const;
	const std::string getPass() const;
	const std::string getTopic() const;
	const std::map<const std::string, Client>::iterator getClientsIt();
	const std::map<const std::string, Client> getClient();
	const std::map<const std::string, int>::iterator getOperators();
	const bool getInvitMode() const;
	const bool getTopicMode() const;
	const bool getKeyMode() const;

	void setName(std::string);
	void setInviteo(bool);
	void setLimit(int);
	void setPass(std::string);
	void setTopic(std::string);
	void setTopicMode(bool);
	void setKeyMode(bool);
	//void addClient();
	//void setTopic(std::string);
	bool IsBanned(const std::string&) const;

};

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

public:
	Client();
	Client& operator=(Client&);
	Client(int clientfd, const std::string hostname);
	~Client();



	//getters
	const std::string Get_host() const;
	const std::string Get_nick() const;
	std::vector<Channel> Get_chan() const;
	const std::string Get_user() const;
	const std::string Get_msg() const;
	const std::string Get_realname() const;
	int	Get_state() const;
	int Get_clfd() const;

	//setters
	void	Set_host(std::string);
	void	Set_nick(std::string);
	void	add_chan(Channel);
	void	Set_chan(std::string);
	void	Set_user(std::string);
	void	Set_msg(std::string);
	void	Set_realname(std::string);
	void	Set_state(int);
	void	addMsg(std::string);

	void	isRegistered();
	const std::string makeCLname() const;
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
	Client getClientName(const std::string&);
	std::vector<Channel>::iterator getChanName(const std::string&); 

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
	//(221)
	static void RPL_UMODEIS(const Client&);
	//(332)
	static void RPL_TOPIC(const Client&, Channel);
	//(353)
	static void RPL_NAMREPLY(const Client&, Channel);
	//(366)
	static void RPL_ENDOFNAMES(const Client&, std::string);
	//(401)
	static void ERR_NOSUCHNICK(const Client&, const std::string&);
	//(431) 
	static void ERR_NONICKNAMEGIVEN(const Client&);
	//(432)
	static void ERR_ERRONEUSNICKNAME(const Client&, const std::string&);
	//(433)
	static void ERR_NICKNAMEINUSE(const Client&, const std::string&);
	//(442)
	static void ERR_NOTONCHANNEL(const Client&, const std::string);
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
};

#endif