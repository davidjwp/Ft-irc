#include "irc.hpp"

//GOOD
void Server::cPING(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	if (msg->find('\r') != std::string::npos) msg->erase(msg->find('\r')); 
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		if (it->Get_nick() == *msg) {cl->reply(" PONG " + *msg); return ;}}
	
	Reply::ERR_NOSUCHSERVER(*cl, *msg);
}