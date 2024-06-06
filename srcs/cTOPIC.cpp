#include "irc.hpp"

void Server::cTOPIC(std::vector<std::string> messages, int fd) {
		std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	unsigned int  num;
	try {num = getChanNum(*msg);}
	catch (std::exception& err) {Reply::ERR_NOSUCHCHANNEL(*cl, *msg); return ;}
	if (!OnChannel(*cl, _channels[num])) {Reply::ERR_NOTONCHANNEL(*cl, *msg); return ;}

	if (_channels[num].getOpMode() && !_channels[num].IsOperator(*cl)) {Reply::ERR_CHANOPRIVSNEEDED(*cl, *msg); return ;}
	
	if (_channels[num].getTopicMode() && !_channels[num].IsOperator(*cl)) {Reply::ERR_CHANOPRIVSNEEDED(*cl, *msg); return ;}

	(msg++)->erase(0);
	std::string topic = "";
	while (msg != messages.end()) {
		topic += *msg;
		topic += " ";
		msg++;
	}
	if(topic.find('\r') != string::npos) 
		topic.erase(topic.find('\r'));

	_channels[num].setTopic(topic);
	_channels[num].setTopicMode(true);

	string rpl = cl->makeCLname() + " 332 " + cl->Get_nick() + " " + _channels[num].getName() + " " + _channels[num].getTopic() + "\r\n";
	std::cout << colstring(Bgreen, rpl) << std::endl;  
	_channels[num].Broadcast(rpl);
}
