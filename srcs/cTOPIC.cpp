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

/*
String      getTopicStr(std::vector<String> params)
{
    String topic;

    unsigned int i = 3;
    topic = params[2];
    if (topic[0] == ':')
        topic = topic.substr(1);
    while (i < params.size())
    {
        topic += " ";
        topic += params[i];
        i++;
    }
    topic = erasebr(topic);
    return topic;
}

int		Server::cmdTopic(std::vector<String> args, Client &cl)
{
	try 
	{
		std::vector<Channel>::iterator chan = findChannelIt(chan_name);
		if (isClientInChannel(*chan, cl.getFd()))
		{
			if (args.size() == 2)
			{
				if (chan->getTopic().empty())
					cl.reply(NO_SET_TOPIC(cl, chan_name));
				else
					cl.reply(RPL_TOPIC(cl, chan_name, chan->getTopic()));
				return (0);
			}
			else if (is_operator_in_Channel(cl, *chan))
			{
				chan->setTopic(getTopicStr(args));
				chan->broadcast(RPL_TOPIC(cl, chan_name, chan->getTopic()));
				return (0);
			}
			else
				cl.reply(ERROR_CHANNEL_OPERATOR_NEEDED(cl, chan_name));
		}
		else
			cl.reply(ERROR_NOT_ON_CHANNEL(cl, chan_name));
	}
	catch (const std::exception& e) 
	{
		cl.reply(ERROR_NO_SUCH_CHANNEL_EXISTS(cl, chan_name));
	}
	return (-1);
}
*/