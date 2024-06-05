#include "irc.hpp"

void Server::cTOPIC(std::vector<std::string> messages, int fd) {
		std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	Channel chan;
	try {chan = *getChanName(*msg);}
	catch (std::exception& err) {Reply::ERR_NOSUCHCHANNEL(*cl, *msg); return ;}
	// cout << "Channel Name: " << chan.getName() << endl;
	if (!OnChannel(*cl, chan)) {Reply::ERR_NOTONCHANNEL(*cl, *msg); return ;}

	if (chan.getOpMode() && !chan.IsOperator(*cl)) {Reply::ERR_CHANOPRIVSNEEDED(*cl, *msg); return ;}

	(msg++)->erase(0);
	// msg++;
	std::string topic = "";
	while (msg != messages.end()) {
		topic += *msg;
		topic += " ";
		msg++;
	}
	if(topic.find('\r') != string::npos)
		topic.erase(topic.find('\r'));

	// cout << "Topic String: " << topic << endl;
	chan.setTopic(topic);
	chan.setTopicMode(true);
	cout << "Chann Topic: " << chan.getTopic() << endl;
	Reply::RPL_TOPIC(*cl, chan);
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