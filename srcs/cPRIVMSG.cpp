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

///connect localhost 4222 pass SHAAP

	//client messages
	Client client;
	try {
		client = getClientName(*msg);

		std::string message = (++msg)->substr(1);
		while (++msg != messages.end()) {
			message += " ";
			message += *msg;
		}
		if (message.find('\r') != std::string::npos) message.erase(message.find('\r'));

		std::string HA = client.makeCLname() + " PRIVMSG " + client.Get_nick() + " :" +message;
		for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++) {
			for (std::map<const std::string, Client>::iterator cls = it->getClient().begin(); cls != it->getClient().end(); cls++)
				send(cls->second.Get_clfd(), HA.c_str(), HA.size(), 0);
		}

		for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
			it->reply(" PRIVMSG " + client.Get_nick() + " :" + message);


		//client.reply(" PRIVMSG " + client.Get_nick() + " :" + message);

		
		return ;
	}
	catch (std::exception& err){}

	//channel messages
	std::vector<Channel>::iterator chans;
	try {chans = getChanName(*msg);}
	catch (std::exception& err){}
	
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
}