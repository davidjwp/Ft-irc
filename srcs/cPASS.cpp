#include "irc.hpp"

void	Server::cPASS(std::vector<std::string> messages, int fd) {
	std::vector<std::string>::iterator msg;
	std::vector<Client>::iterator cl = getClientit(fd);

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	if (cl->Get_state() & REG) {
		Reply::ERR_ALREADYREGISTERED(*cl);
		return ;
	}

	if (msg->find('\r') != std::string::npos) msg->erase(msg->find('\r'));
	if (*msg != _pass) Reply::ERR_PASSWDMISMATCH(*cl);
	cl->Set_state(PASS);
	cl->isRegistered();
}
