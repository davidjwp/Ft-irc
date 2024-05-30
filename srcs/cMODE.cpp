#include "irc.hpp"

//WORK IN PROGRESS

void	Reply::ERR_NOSUCHNICK(const Client& cl, const std::string& nickorchan){cl.reply(" 401 " + cl.Get_nick() + " " + nickorchan + " :No such nick/channel");}

void	Reply::ERR_USERSDONTMATCH(const Client& cl) {cl.reply(" 501 " + cl.Get_nick() + " :Cant change mode for other users");}

void	Reply::ERR_NOTREGISTERED(const Client& cl) {cl.reply(" 451 " + cl.Get_nick() + " :You have not registered");}

void	Reply::ERR_NOTONCHANNEL(const Client& cl) {cl.reply(" 442 " + cl.Get_nick() + " " + cl.Get_chan() + " :You're not on that channel");}

//void	Reply::ERR_UNKNOWNMODE(const Client& cl){}

//void	Reply::RPL_UMODEIS(const Client& cl) {cl.reply(" 221 " + cl.Get_nick() + " :")}

void	Server::cMODE(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}

	if (!cl->Get_chan().size()) {Reply::ERR_NOTONCHANNEL(*cl); return ;}

	if (messages.size() > 2)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	if (*msg == cl->Get_nick()) Reply::ERR_USERSDONTMATCH(*cl);



}