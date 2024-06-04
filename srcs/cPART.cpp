#include "irc.hpp"

void Server::cPART(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}

	while (msg != messages.end()) {
		if (msg->find('\r') != std::string::npos) msg->erase(msg->find('\r'));

		try {
			std::vector<Channel>::iterator chan = getChanName(*msg);

			std::vector<Channel>::iterator it = cl->Get_chan().begin();
			while (it != cl->Get_chan().end()) {
				if (it->getName() == chan->getName()) break ;
				it++;
			}
			cl->Get_chan().erase(it);//this
			chan->EraseClient(*cl);
			if (chan->getClient().empty())
				_channels.erase(chan);
		}
		catch (std::exception& err){Reply::ERR_NOSUCHCHANNEL(*cl, *msg);}
		msg++;
	}
}
