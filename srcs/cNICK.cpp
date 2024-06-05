#include "irc.hpp"

//GOOD
void Server::cNICK(std::vector<std::string> messages, int fd){
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NONICKNAMEGIVEN(*cl); return ;}
	
	if (msg->find('\r') != std::string::npos) msg->erase(msg->find('\r'));
	
	if (cl->Get_state() & REG) {Reply::ERR_ALREADYREGISTERED(*cl); return ;}

	for (struct {const char* forbidden; int i;} loop = {" ,*?!@.", 0}; loop.forbidden[loop.i] != 0; loop.i++) {
		if (msg->find(loop.forbidden[loop.i], 0) != std::string::npos) {
			Reply::ERR_ERRONEUSNICKNAME(*cl, *msg);
			return ;
		}
	}
	if ((*msg)[0] == '#' || (*msg)[0] == '&' || (*msg)[0] == ':') {
		Reply::ERR_ERRONEUSNICKNAME(*cl, *msg);
		return ;
	}

	if ((cl->Get_state() & NICK) && *msg == cl->Get_nick()) {
		Reply::ERR_NICKNAMEINUSE(*cl, *msg);
		return ;
	}

	for (std::vector<Client>::iterator clients = _clients.begin(); clients != _clients.end(); clients++)
		if (clients->Get_nick() == *msg) {Reply::ERR_NICKNAMEINUSE(*cl, *msg); return ;}

	cl->Set_nick(*msg);
	cl->reply(" NICK :" + *msg);
	cl->isRegistered();
}