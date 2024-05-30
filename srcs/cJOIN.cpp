#include "irc.hpp"

//WORK IN PROGRESS
void	Reply::ERR_BADCHANMASK(const Client& cl, const std::string& chan) {cl.reply(" 476 " + chan + " :Bad Channel Mask");}

void Server::cJOIN(std::vector<std::string> messages, int fd){
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}

	std::vector<std::string> channels;
	std::stringstream stream(*msg);
	std::string tmp;

	while (std::getline(stream, tmp, ',')) {channels.push_back(tmp);}

	if (msg->at(0) != '#') { Reply::ERR_BADCHANMASK(*cl, *msg); return ;}

	//for (struct {const char* forbidden; int i;} loop = {" ,\a", 0}; loop.forbidden[loop.i] != 0; loop.i++) {
	//	if (msg->find(loop.forbidden[loop.i], 0) != std::string::npos) {
	//		Reply::ERR_BADCHANMASK(*cl, *msg);
	//		return ;
	//	}
	//}


}