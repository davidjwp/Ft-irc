#include "irc.hpp"

//(001)
void Reply::RPL_WELCOME(const Client& cl){ cl.reply(" 001 " + cl.Get_nick() + " :Welcome to the Internet Relay Network, " + cl.Get_nick());}

//(221)
void	Reply::RPL_UMODEIS(const Client& cl) {
	std::string modes;
	if (cl.Get_state() & INV) modes += " +i ";
	cl.reply(" 221 " + cl.Get_nick() + " :" + modes);
}

void	Reply::RPL_CHANNELMODEIS(const Client& cl, const Channel& chan) {
	std::string msg = " 324 " + cl.Get_nick() + " " + chan.getName() + " +";
	if (chan.getInvitMode()) msg += 'i';
	if (chan.getTopicMode()) msg += 't';
	if (chan.getKeyMode()) msg += 'k';
	if (chan.getOpMode()) msg += 'o';
	if (chan.getLimit() != -1) msg += 'l';
	cl.reply(msg);
}

//(332)
void	Reply::RPL_TOPIC(const Client& cl, Channel& chan) {cl.reply(" 332 " + cl.Get_nick() + " " + chan.getName() + " " + chan.getTopic());}

//(341)
void	Reply::RPL_INVITING(const Client& cl, const std::string nick, Channel& chan) {cl.reply(" 341 " + cl.Get_nick() + " " + nick + " " + chan.getName());}

//(353)
void	Reply::RPL_NAMREPLY(const Client& cl, Channel& chan) {
	std::string msg = " 353 " + cl.Get_nick() + " = " + chan.getName() + " :";
	std::string chan_clients;
	unsigned int size = chan.getClient().size();
	std::map<const std::string, Client>::iterator it = chan.getClientsIt();
	while (size--) {
		if (chan.IsOperator(it->second))
			chan_clients += "@" + (it++)->second.Get_nick() + " ";
		else
			chan_clients += (it++)->second.Get_nick() + " ";
	}
	cl.reply(msg + chan_clients);
}

//(366)
void	Reply::RPL_ENDOFNAMES(const Client& cl, std::string chan) {cl.reply(" 366 " + cl.Get_nick() + " " + chan + " :End of /NAMES list.");}

//(381)
void	Reply::RPL_YOUREOPER(const Client& cl) {cl.reply(" 381 " + cl.Get_nick() + " :You are now an IRC operator");}

//(401)
void	Reply::ERR_NOSUCHNICK(const Client& cl, const std::string& nickorchan){cl.reply(" 401 " + cl.Get_nick() + " " + nickorchan + " :No such nick/channel");}

//(402)
void	Reply::ERR_NOSUCHSERVER(const Client& cl, const std::string& server) {cl.reply(" 402 " + cl.Get_nick() + " " + server + " :No such server");}

//(403)
void	Reply::ERR_NOSUCHCHANNEL(const Client& cl, const std::string& chan) {cl.reply(" 403 " + cl.Get_nick() + " " + chan + ":No such channel");}

//(431)
void	Reply::ERR_NONICKNAMEGIVEN(const Client& cl) { cl.reply(" 431 " + cl.Get_nick() + " :No nickname given");}

//(432)
void	Reply::ERR_ERRONEUSNICKNAME(const Client& cl, const std::string& err) { cl.reply(" 432 " + cl.Get_nick() + " " + err + " :Erroneus nickname");}

//(433)
void	Reply::ERR_NICKNAMEINUSE(const Client& cl, const std::string& err) { cl.reply(" 433 " + cl.Get_nick() + " " + err + " :Nickname is already in use");}

//(441)
void	Reply::ERR_USERNOTINCHANNEL(const Client& cl, const std::string nick, const Channel& chan) {cl.reply(" 441 " + cl.Get_nick() + " " + nick + " " + chan.getName() + " :They aren't on that channel");}

//(442)
void	Reply::ERR_NOTONCHANNEL(const Client& cl, const std::string chan) {cl.reply(" 442 " + cl.Get_nick() + " " + chan + " :You're not on that channel");}

//(443)
void	Reply::ERR_USERONCHANNEL(const Client& cl, const std::string nick, Channel& chan) {cl.reply(" 443 " + cl.Get_nick() + " " + nick + " " + chan.getName() + " :is already on channel");}

//(451)
void	Reply::ERR_NOTREGISTERED(const Client& cl) {cl.reply(" 451 " + cl.Get_nick() + " :You have not registered");}

//(461)
void	Reply::ERR_NEEDMOREPARAMS(const Client& client, const std::string& com){ client.reply(" 461 " + client.Get_nick() + " " + com + " :Not enough parameters");}

//(462)
void	Reply::ERR_ALREADYREGISTERED(const Client& client){ client.reply( " 462 " + client.Get_nick()+ " :You may not reregister");}

//(464)
void	Reply::ERR_PASSWDMISMATCH(const Client& cl) {cl.reply(" 464 " + cl.Get_nick() + " :Password incorrect");}

//(471)
void	Reply::ERR_CHANNELISFULL(const Client& cl, std::string chan) {cl.reply(" 471 " + cl.Get_nick() + " " + chan + " :Cannot join channel (+l)");}

//472`
//void	Reply::ERR_UNKNOWNMODE(const Client& cl){}

//(473)
void	Reply::ERR_INVITEONLYCHAN(const Client& cl, std::string chan) {cl.reply(" 473 " + cl.Get_nick() + " " + chan + " :Cannot join channel (+i)");}

//(474)
void	Reply::ERR_BANNEDFROMCHAN(const Client& cl, std::string chan) {cl.reply(" 474 " + cl.Get_nick() + " " + chan + " :Cannot join channel (+b)");}

//(475)
void	Reply::ERR_BADCHANNELKEY(const Client& cl, std::string chan) {cl.reply(" 475 " + cl.Get_nick() + " " + chan + " :Cannot join channel (+k)");}

//(476)
void	Reply::ERR_BADCHANMASK(const Client& cl, const std::string& chan) {cl.reply(" 476 " + chan + " :Bad Channel Mask");}

//(482)
void	Reply::ERR_CHANOPRIVSNEEDED(const Client& cl, const std::string chan) {cl.reply(" 482 " + cl.Get_nick() + " " + chan + " :You're not channel operator");}

//(501)
void	Reply::ERR_UMODEUNKNOWNFLAG(const Client& cl) {cl.reply(" 501 " + cl.Get_nick() + " :Unknown MODE flag");}

//(502)
void	Reply::ERR_USERSDONTMATCH(const Client& cl) {cl.reply(" 502 " + cl.Get_nick() + " :Cant change mode for other users");}

//(525)
void	Reply::ERR_INVALIDKEY(const Client& cl, const Channel& chan) {cl.reply(" 525 " + cl.Get_nick() + " " + chan.getName() + " :Key is not well-formed");}
