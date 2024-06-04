#ifndef CHANNEL_HPP
# define CHANNEL_HPP

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

using std::string;
class Channel {
private:
	string	_name;


	int	_limit;
	string _pass;
	string _topic;
	bool 	_invit_mode;
	bool	_topic_mode;
	bool	_key_mode;
	bool	_op_mode;
	std::map<const string, Client> _clients;
	std::map<const string, Client>	_operators;
	std::vector<string> _banned;
public:
	Channel();
	Channel(const string& chanName, Client client);

	const string getName() const;
	bool getInvit() const;
	int getLimit() const;
	const string getPass() const;
	const string getTopic() const;
	const std::map<const string, Client>::iterator getClientsIt();
	std::map<const string, Client>& getClient();
	const std::map<const string, Client>::iterator getOperators();
	bool getInvitMode() const;
	bool getTopicMode() const;
	bool getKeyMode() const;
	bool getOpMode() const;

	void AddClient(Client&);
	bool IsOperator(const Client&) const;
	void setName(string);
	void setInviteo(bool);
	void setLimit(int);
	void setPass(string);
	void setTopic(string);
	void setTopicMode(bool);
	void setKeyMode(bool);
	void addOper(Client&);
	void remOper(Client&);
	//void addClient();
	//void setTopic(string);
	bool IsBanned(const string&);
	void Broadcast(string, int) const;
	void Broadcast(string) const;
	void EraseClient(Client&);
};

#endif