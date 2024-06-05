#include "../includes/irc.hpp"

void	Server::cQUIT(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	(void)messages;
	std::vector<Channel> chans;
	if (!((chans = cl->Get_chan()).empty())) {
		string clientChannels = cl->Get_chan().begin()->getName();
		for (std::vector<Channel>::iterator it = chans.begin() + 1; it != chans.end(); it++)
			clientChannels += "," + it->getName();
		std::vector<string> vec;
		vec.push_back("PART");
		vec.push_back(clientChannels);
		cPART(vec, fd);
		Disconnect_client(fd);
	}
}