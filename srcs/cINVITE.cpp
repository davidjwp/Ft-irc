#include "irc.hpp"

void Server::cINVITE(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}

	if (messages.size() > 2)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	if ((msg + 1)->find('\r') != std::string::npos) (msg + 1)->erase((msg + 1)->find('\r'));
	
	unsigned int num;
	try {num = getClientNum(*msg);}
	catch (std::exception& err) {Reply::ERR_NOSUCHNICK(*cl, *msg); return ;}

	std::vector<Channel>::iterator chan;
	try {chan = getChanName(*(++msg));}
	catch (std::exception& err) {Reply::ERR_NOSUCHCHANNEL(*cl, *msg); return ;}
	
	if (chan->getClient().find(cl->Get_nick()) == chan->getClient().end()) {Reply::ERR_NOTONCHANNEL(*cl, chan->getName()); return ;}

	if (!chan->IsOperator(*cl) && chan->getInvitMode()) {Reply::ERR_CHANOPRIVSNEEDED(*cl, chan->getName()); return ;}

	if (chan->getClient().find(*msg) != chan->getClient().end()) {Reply::ERR_NOTONCHANNEL(*cl, chan->getName()); return ;}

	if (chan->getLimit() != -1 && (size_t)chan->getLimit() >= chan->getClient().size()) {Reply::ERR_CHANNELISFULL(*cl, chan->getName()); return ;}
	
	_clients[num].Set_InvitedChannel(chan->getName());
	_clients[num].reply(" you are invited to " + chan->getName());
	Reply::RPL_INVITING(*cl, _clients[num].Get_nick(), *chan);
}