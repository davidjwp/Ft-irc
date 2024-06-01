#include "irc.hpp"

//WORK IN PROGRESS 

static void SendMessage(std::string message, Channel& chan, Client& cl) {
	message.insert(0, cl.makeCLname() + " PRIVMSG " + chan.getName() + " :");
	chan.Broadcast(message, cl.Get_clfd());
}

void Server::cPRIVMSG(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}

	if (messages.size() > 2)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	std::vector<Channel>::iterator chan;
	if (msg->at(0) == '#') {
		try {
			chan = getChanName(*msg);
			std::string message = (++msg)->substr(1);

			while (++msg != messages.end()) {
				message += " ";
				message += *msg;
			}
			if (message.find('\r') != std::string::npos) message.erase(message.find('\r'));

			SendMessage(message, *chan, *cl);
		}
		catch (std::exception& err) { Reply::ERR_NOTONCHANNEL(*cl, *msg); return ;}//might not need to be part of the channel
	}

	//chan->Broadcast(message, cl->Get_clfd());
}