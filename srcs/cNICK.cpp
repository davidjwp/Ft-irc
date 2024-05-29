#include "irc.hpp"

void Reply::ERR_NONICKNAMEGIVEN(const Client& cl) { cl.reply(" 431 " + cl.Get_nick() + " :No nickname given");}

void Reply::ERR_ERRONEUSNICKNAME(const Client& cl, const std::string& err) { cl.reply(" 432 " + cl.Get_nick() + " " + err + ":Erroneous nickname");}

void Reply::ERR_NICKNAMEINUSE(const Client& cl, const std::string& err) { cl.reply(" 433 " + cl.Get_nick() + " " + err + ":Nickname is already in use");}

void Server::cNICK(std::vector<std::string> messages, int fd){
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NONICKNAMEGIVEN(*cl); return ;}

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

	cl->Set_nick(*msg);
	cl->isRegistered();
}