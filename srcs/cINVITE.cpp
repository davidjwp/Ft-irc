#include "irc.hpp"

void Server::cINVITE(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	if ((msg + 1)->find('\r') != std::string::npos) (msg + 1)->erase((msg + 1)->find('\r'));
	std::vector<Channel>::iterator chan;
	try {chan = getChanName(*(msg + 1));}
	catch (std::exception& err) {Reply::ERR_NOSUCHCHANNEL(*cl, *(msg + 1)); return ;}

	if (chan->getClient().find(cl->Get_nick()) == chan->getClient().end()) {Reply::ERR_NOTONCHANNEL(*cl, chan->getName()); return ;}

	if (!chan->IsOperator(*cl) && chan->getInvitMode()) {Reply::ERR_CHANOPRIVSNEEDED(*cl, chan->getName()); return ;}

	if (chan->getClient().find(*msg) != chan->getClient().end()) {Reply::ERR_NOTONCHANNEL(*cl, chan->getName()); return ;}

	if (chan->getLimit() != -1 && (size_t)chan->getLimit() >= chan->getClient().size()) {Reply::ERR_CHANNELISFULL(*cl, chan->getName()); return ;}
	Reply::RPL_INVITING(*cl, *msg, *chan);
	chan->AddClient(*cl);
	cl->add_chan(*chan);
	chan->Broadcast(std::string( cl->makeCLname() + " Join :" + cl->Get_nick()));
	//cl->reply(" Join :" + *it);
	Reply::RPL_TOPIC(*cl, *chan);
	Reply::RPL_NAMREPLY (*cl, *chan);
	Reply::RPL_ENDOFNAMES(*cl, chan->getName());
}