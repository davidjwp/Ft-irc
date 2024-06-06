#ifndef SERVER_HPP
# define SERVER_HPP

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

using std::string;
using std::vector;
using std::cout;
using std::endl;
class Client;
class Channel;

class Server {
private:
	string	_host;
	string _pass;
	string	_opass;

	int	_port;
	int	_sock;

	vector<pollfd> _pollfds;
	vector<Client> _clients;
	vector<Channel> _channels;
public:
	~Server();
	Server(int port, const string &pass);

	int 	Start_server();
	void	Add_client();
	void	Clients_Status();
	void	Client_messages(int);
	string Get_message(int fd);
	void	Proc_message(string, int);
	vector<Client>::iterator	getClientit(int fd);
	vector<pollfd>::iterator	getPollfd(int fd);
	vector<string> split(string);
	void	Disconnect_client(int);
	Client& getClientName(const string&);
	unsigned int getClientNum(const string&);
	vector<Channel>::iterator getChanName(const string&); 
	unsigned int getChanNum(const  string&) ;
	void ChangeOper(bool, Channel&, Client&);
	void ChangePassword(bool, string, Client&, Channel&) const;
	void Change_topic(bool, Client&, Channel&) const;
	void ChangeInvito(bool, Client&, Channel&) const;
	void ChangeLimit(bool, string, Channel&, Client&) const;
	bool OnChannel(Client&, Channel&) const;
	void AddChannel(Channel&);
	Client& GetClient(int);

	//getters

	string Get_host();
	string Get_pass();
	string Get_opass();

	void	cNICK(vector<string>, int);
	void	cUSER(vector<string>, int);
	void	cPASS(vector<string>, int);
	void	cPRIVMSG(vector<string>, int);
	void	cJOIN(vector<string>, int);
	void	cWHO(vector<string>, int);
	void	cPING(vector<string>, int);
	void	cOPER(vector<string>, int);
	void	cPART(vector<string>, int);
	void	cMODE(vector<string>, int);
	void	cKICK(vector<string>, int);
	void	cINVITE(vector<string>, int);
	void	cNOTICE(vector<string>, int);
	void	cTOPIC(vector<string>, int);
	void	cKILL(vector<string>, int);
	void	cLIST(vector<string>, int);
	void	cQUIT(vector<string>, int);


	static void	reply(Client, string);
};

#endif