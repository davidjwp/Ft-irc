#include "irc.hpp"

//WORK IN PROGRESS
void	Reply::ERR_BADCHANMASK(const Client& cl, const std::string& chan) {cl.reply(" 476 " + chan + " :Bad Channel Mask");}

void	Reply::RPL_TOPIC(const Client& cl, Channel chan) {cl.reply(" 332 " + cl.Get_nick() + " " + chan.getName() + " : " + chan.getTopic());}

void	Reply::RPL_NAMREPLY(const Client& cl, Channel chan) {
	std::string msg = " 353 " + cl.Get_nick() + " = " + chan.getName() + " :";
	std::string chan_clients;
	unsigned int size = chan.getClient().size();
	std::map<const std::string, Client>::iterator it = chan.getClientsIt();
	while (size--)
		chan_clients += (it++)->second.Get_nick() + " ";
	cl.reply(msg + chan_clients);
}

void	Reply::RPL_ENDOFNAMES(const Client& cl, std::string chan) {cl.reply(" 366 " + cl.Get_nick() + " " + chan + " :End of /NAMES list");}

void	Reply::ERR_INVITEONLYCHAN(const Client& cl, std::string chan) {cl.reply(" 473 " + cl.Get_nick() + " " + chan + " :Cannot join channel (+i)");}

void	Reply::ERR_CHANNELISFULL(const Client& cl, std::string chan) {cl.reply(" 471 " + cl.Get_nick() + " " + chan + " :Cannot join channel (+l)");}

void	Reply::ERR_BADCHANNELKEY(const Client& cl, std::string chan) {cl.reply(" 475 " + cl.Get_nick() + " " + chan + " :Cannot join channel (+k)");}

void	Reply::ERR_BANNEDFROMCHAN(const Client& cl, std::string chan) {cl.reply(" 474 " + cl.Get_nick() + " " + chan + " :Cannot join channel (+b)");}

static int ChanClientAmount(Channel chan) {
	unsigned int limit = 0;

	for (std::map<const std::string, Client>::iterator it = chan.getClientsIt(); it != chan.getClient().end(); it++)
		limit++;
	return limit;
}

void Server::cJOIN(std::vector<std::string> messages, int fd){
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}

	std::vector<std::string> chanNames;
	
	{
		std::string tmp;
		std::stringstream stream(*msg);
		while (std::getline(stream, tmp, ',')) {
		if (tmp.find('\r') != std::string::npos) tmp.erase(tmp.find('\r')); 	
			chanNames.push_back(tmp);
		}
	}

	bool keys = false;
	std::vector<std::string> chanKeys(chanNames.size());
	if (++msg != messages.end()) {
		keys = true;
		std::string tmp;
		std::stringstream stream(*msg);
		while (std::getline(stream, tmp, ',') && chanNames.size() >= chanKeys.size()) {chanKeys.push_back(tmp);}
	}

	std::vector<std::string>::iterator keyIterator = chanKeys.begin();
	for (std::vector<std::string>::iterator it = chanNames.begin(); it != chanNames.end(); it++) {
		if (it->at(0) != '#') { Reply::ERR_BADCHANMASK(*cl, *msg); return ;}

		std::vector<Channel>::iterator chans; 
		try {chans = getChanName(*it);}
		catch (std::exception& err){ 
		//channel does not exist
			if (it->find('\r') != std::string::npos) it->erase(it->find('\r'));
			
			if ( it->find(',') != std::string::npos || \
				it->find('\a') != std::string::npos || \
				it->find(' ') != std::string::npos || \
				it->size() == 1) {Reply::ERR_BADCHANMASK(*cl, *it); continue ;}
			
			Channel newchan(*it, *cl);
			if (keys && keyIterator != chanKeys.end()) {
				if (keyIterator->size()) {newchan.setPass(*keyIterator); newchan.setKeyMode(true);}
				keyIterator++;
			}
			cl->add_chan(newchan);
			cl->reply(" Join :" + *it);//channel join reply
			Reply::RPL_TOPIC(*cl, newchan);
			Reply::RPL_NAMREPLY (*cl, newchan);
			Reply::RPL_ENDOFNAMES(*cl, newchan.getName());
			Server::_channels.push_back(newchan);//careful with Server::
			continue ;
		}
		//channel exists
		if (chans->getInvit() == true) {Reply::ERR_INVITEONLYCHAN(*cl, chans->getName()); continue ;}

		if (chans->getLimit() != -1 && chans->getLimit() <= ChanClientAmount(*chans)) {Reply::ERR_CHANNELISFULL(*cl, chans->getName()); continue ;}

		if (keys && chans->getKeyMode() && *keyIterator != chans->getPass()) {Reply::ERR_BADCHANNELKEY(*cl, chans->getName()); continue ;}

		if (chans->IsBanned(cl->Get_nick())) {Reply::ERR_BANNEDFROMCHAN(*cl, chans->getName()); continue ;}

		if (keys && keyIterator != chanKeys.end()) {
			if (keyIterator->size()) chans->setPass(*keyIterator);
			keyIterator++;
		}
		cl->add_chan(*chans);
		Reply::RPL_TOPIC(*cl, *chans);
		Reply::RPL_NAMREPLY (*cl, *chans);
		Reply::RPL_ENDOFNAMES(*cl, chans->getName());
	}
}