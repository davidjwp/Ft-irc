#include "irc.hpp"

void Server::cOPER(std::vector<std::string> messages, int fd){
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	std::string pass = *(++msg);
	while (++msg != messages.end()) {
		pass += " ";
		pass += *msg;
	}
	if (pass.find('\r') != std::string::npos) pass.erase(pass.find('\r'));

	if (pass != _opass) Reply::ERR_PASSWDMISMATCH(*cl);
	else { cl->Set_state(OPER); Reply::RPL_YOUREOPER(*cl);}
}