#include "irc.hpp"

void Server::cINVITE(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}

	if (messages.size() > 2)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	if ((msg + 1)->find('\r') != std::string::npos) (msg + 1)->erase((msg + 1)->find('\r'));
	
	Client client;
	try {client = getClientName(*(msg));}
	catch (std::exception& err) {Reply::ERR_NOSUCHNICK(*cl, *msg); return ;}

	std::vector<Channel>::iterator chan;
	try {chan = getChanName(*(++msg));}
	catch (std::exception& err) {Reply::ERR_NOSUCHCHANNEL(*cl, *msg); return ;}
	
	if (chan->getClient().find(cl->Get_nick()) == chan->getClient().end()) {Reply::ERR_NOTONCHANNEL(*cl, chan->getName()); return ;}

	if (!chan->IsOperator(*cl) && chan->getInvitMode()) {Reply::ERR_CHANOPRIVSNEEDED(*cl, chan->getName()); return ;}

	if (chan->getClient().find(*msg) != chan->getClient().end()) {Reply::ERR_NOTONCHANNEL(*cl, chan->getName()); return ;}

	if (chan->getLimit() != -1 && (size_t)chan->getLimit() >= chan->getClient().size()) {Reply::ERR_CHANNELISFULL(*cl, chan->getName()); return ;}
	

	//chan->AddClient(*cl);//might not work 
	//client.add_chan(*chan);
	//client.reply("JOIN " + chan->getName());
	std::vector<string> vec;
	Reply::RPL_INVITING(*cl, client.Get_nick(), *chan);
	vec.push_back("JOIN");
	vec.push_back(chan->getName());
	cJOIN(vec, client.Get_clfd());
	//chan->Broadcast(std::string( client.makeCLname() + " Join :" + client.Get_nick()));
	//client.reply(" Join :" + client.Get_nick());
	//Reply::RPL_TOPIC(client, *chan);
	//Reply::RPL_NAMREPLY (client, *chan);
	//Reply::RPL_ENDOFNAMES(client, chan->getName());
}

//INVITE PO7 #test

//INVITE PO6 #test
//:PO6!djacobs@localhost 332 PO6 #test 

//:PO6!djacobs@localhost 353 PO6 = #test :PO @PO5 PO6 PO7 

//:PO6!djacobs@localhost 366 PO6 #test :End of /NAMES list.


//JOIN #test
//:PO6!djacobs@localhost 332 PO6 #test 

//:PO6!djacobs@localhost 353 PO6 = #test :PO @PO5 PO6 PO7 

//:PO6!djacobs@localhost 366 PO6 #test :End of /NAMES list.