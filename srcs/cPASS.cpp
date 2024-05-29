#include "irc.hpp"

void Reply::ERR_PASSWDMISMATCH(const Client& cl) {cl.reply(" 464 " + cl.Get_nick() + " :Password incorrect");}

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

	if (*msg != _pass) Reply::ERR_PASSWDMISMATCH(*cl);
	cl->Set_state(PASS);
}

/*
ERR_NEEDMOREPARAMS (461)
ERR_ALREADYREGISTERED (462)
ERR_PASSWDMISMATCH (464)
*/