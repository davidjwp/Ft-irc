#include "../includes/irc.hpp"

void	Server::cQUIT(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	(void)messages;
	std::vector<Channel> chans;
	if (!((chans = cl->Get_chan()).empty())) {
		std::vector<Channel>::iterator it = chans.begin();
		string clientChannels = it->getName();
		while ( ++it != chans.end())
			clientChannels += "," + it->getName();
		std::vector<string> vec;
		vec.push_back("PART");
		vec.push_back(clientChannels);
		cPART(vec, fd);
		Disconnect_client(fd);
	}
}