#include "irc.hpp"

void Server::cKICK(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	//making sure that you have proper channel permissions 
	Channel chan;
	try {chan = *getChanName(*msg);}
	catch(std::exception& err){Reply::ERR_NOSUCHCHANNEL(*cl, *msg); return ;}
	if (msg + 1 == messages.end()) {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}
	if (!OnChannel(*cl, chan)) {Reply::ERR_NOTONCHANNEL(*cl, *msg); return ;}
	if (!chan.IsOperator(*cl)) {Reply::ERR_CHANOPRIVSNEEDED(*cl, *msg); return ;}
	msg++;

	Client client;
	try{client = getClientName(*msg);}
	catch(std::exception& err){Reply::ERR_NOSUCHNICK(*cl, *msg); return ;}
	if (!OnChannel(client, chan)) {Reply::ERR_USERNOTINCHANNEL(*cl, client.Get_nick(), chan); return ;}

	msg->erase(0);
	std::string kick_message;
	while (msg != messages.end()) {
		if ((msg)->find('\r') != std::string::npos) msg->erase(msg->find('\r'));

		kick_message += *msg;
		kick_message += " ";
		msg++;
	}
	if (kick_message.size())
		chan.Broadcast("KICK " + chan.getName() + " " + client.Get_nick() + " " + cl->Get_nick() + "  :" + kick_message);
	else
		chan.Broadcast("KICK " + chan.getName() + " " + client.Get_nick() + " " + cl->Get_nick() );
	std::vector<std::string> msgs;
	msgs.push_back("PART");
	msgs.push_back(chan.getName());
	cPART(msgs, client.Get_clfd());
}