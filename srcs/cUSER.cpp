#include "irc.hpp"

//DONE
void	Server::cUSER(std::vector<std::string> messages, int fd){
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 5)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	if (cl->Get_state() & REG) Reply::ERR_ALREADYREGISTERED(*cl); 

	cl->Set_user(*msg++);
	while (msg->find(':') != 0)
		msg++;

	if (msg == messages.end()) {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}
	
	if (msg->size() != 1) {
		std::string realname = &(msg->at(1));
		while (++msg != messages.end()) {
			realname += (" " + *msg);
		}
		if (realname.find('\r') != std::string::npos) realname.erase(realname.find('\r'));
		cl->Set_realname(realname);
	}
	cl->isRegistered();
}