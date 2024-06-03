#include "irc.hpp"


//WORK IN PROGRESS LOOK FOR CHANNEL OPERATOR PART AT THE END
void Server::cOPER(std::vector<std::string> messages, int fd){
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	std::string pass = *(++msg);
	while (++msg != messages.end()) {
		pass += " ";
		pass += *msg;
	}
	if (pass.find('\r') != std::string::npos) pass.erase(pass.find('\r'));

	if (pass != _opass) Reply::ERR_PASSWDMISMATCH(*cl);
	else { 
		cl->Set_state(OPER);
		//if (cl->Get_chan().size())
		//	for (std::vector<Channel>::iterator )

		for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
			if (it->getClient().find(cl->Get_nick()) != it->getClient().end()) it->addOper(*cl);

		std::vector<Channel> clientChannels = cl->Get_chan();
		for (std::vector<Channel>::iterator it = clientChannels.begin(); it != clientChannels.end(); it++)
			it->addOper(*cl);
			
		Reply::RPL_YOUREOPER(*cl);
	}
}