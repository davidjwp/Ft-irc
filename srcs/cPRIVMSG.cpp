#include "irc.hpp"

//WORK IN PROGRESS 

void Server::cPRIVMSG(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}

	if (messages.size() > 2)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	try { std::vector<Channel>::iterator chan = getChanName(*msg);}
	catch (std::exception& err) { Reply::ERR_NOTONCHANNEL(*cl, *msg); return ;}

	std::string message = (++msg)->substr(1);
	while (msg != messages.end()) {
		message += " ";
		message += *msg;
		msg++;
	}
	if (message.find('\r') != std::string::npos) message.erase(message.find('\r'));

	
}