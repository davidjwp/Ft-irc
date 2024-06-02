#include "irc.hpp"

void Server::cKICK(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	//making sure that you have proper channel permissions 
	Channel chan;
	try {chan = *getChanName(*msg);}
	catch(std::exception& err){Reply::ERR_NOSUCHCHANNEL(*cl, *msg); return ;}
	if (msg + 1 == messages.end()) {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}
	if (!OnChannel(*cl, chan)) {Reply::ERR_NOTONCHANNEL(*cl, *msg); return ;}
	if (chan.getOpMode() && chan.IsOperator(*cl)) {Reply::ERR_CHANOPRIVSNEEDED(*cl, *msg); return ;}
	msg++;

	if ()

	while (msg != messages.end()) {
		if ((msg)->find('\r') != std::string::npos) msg->erase(msg->find('\r'));

		if (cl->Get_state() & OPER) {Reply::ERR_CHANOPRIVSNEEDED(*cl, *msg); return ;}
		
		msg++;
	}
}
/*
int	Server::cmdKick(std::vector<String> args, Client &cl) 
{
	String cmd = args.at(0);
	if (args.size() < 3)
	{
		cl.reply("461 " + cl.getNickname() + " " + cmd + " :Not enough parameters");
		return -1;
	}
	if (isChannel(args.at(1)) == false)
	{
		cl.reply("403 " + cl.getNickname() + " " + args.at(1) + " :No such channel");
		return -1;
	}
	if (isClientInChannel(findChannel(args.at(1)), cl.getFd()) == false)
	{
		cl.reply("442 " + cl.getNickname() + " " + args.at(1) + " :You're not on that channel");
		return -1;
	}
	if (cl.getFd() != findChannel(args.at(1)).getFdOp())
	{
		cl.reply(ERROR_CHANNEL_OPERATOR_NEEDED(cl, args.at(1)));
		return -1;
	}
	if (is_client_not_in_Channel(findChannel(args.at(1)), erasebr(args.at(2))) == false)
	{
		cl.reply("441 " + cl.getNickname() + " " + erasebr(args.at(2)) + " " + args.at(1) + " :They aren't on that channel");
		return -1;
	}
	std::vector<String> tmp;
	tmp.push_back("Part");
	tmp.push_back(args.at(1));
	cmdPart(tmp, findClient(args.at(2)));
	return 0;
}*/