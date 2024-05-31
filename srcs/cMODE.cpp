#include "irc.hpp"

//WORK IN PROGRESS

void	Reply::ERR_NOSUCHNICK(const Client& cl, const std::string& nickorchan){cl.reply(" 401 " + cl.Get_nick() + " " + nickorchan + " :No such nick/channel");}

void	Reply::ERR_USERSDONTMATCH(const Client& cl) {cl.reply(" 501 " + cl.Get_nick() + " :Cant change mode for other users");}

void	Reply::ERR_NOTREGISTERED(const Client& cl) {cl.reply(" 451 " + cl.Get_nick() + " :You have not registered");}

void	Reply::ERR_NOTONCHANNEL(const Client& cl, const std::string chan) {cl.reply(" 442 " + cl.Get_nick() + " " + chan + " :You're not on that channel");}

void	Reply::ERR_CHANOPRIVSNEEDED(const Client& cl, const std::string chan) {cl.reply(" 482 " + cl.Get_nick() + " " + chan + " :You're not channel operator");}

//void	Reply::ERR_UNKNOWNMODE(const Client& cl){}

void	Reply::ERR_UMODEUNKNOWNFLAG(const Client& cl) {cl.reply(" 501 " + cl.Get_nick() + " :Unknown MODE flag");}

void	Reply::RPL_UMODEIS(const Client& cl) {
	std::string modes;
	if (cl.Get_state() & INV) modes += " +i ";
	cl.reply(" 221 " + cl.Get_nick() + " :" + modes);
}

static bool Check_mod_ops(
	std::vector<std::string>::iterator msgs, 
	std::vector<std::string>::iterator end,
	std::vector<Client>::iterator cl) {
	while (msgs != end) {
		if (msgs->find('\r') != std::string::npos) msgs->erase(msgs->find('\r'));

		if (msgs->size() != 2) {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return false;}

		if (msgs->at(0) != '-' && msgs->at(0) != '+') {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return false;}

		if (msgs->at(1) != 'i' && msgs->at(1) != 'k' && msgs->at(1) != 'l' && msgs->at(1) != 'o' && msgs->at(1) != 't')
		{Reply::ERR_UMODEUNKNOWNFLAG(*cl); return false;}
		msgs++;
	}
	return true;
}

void  Change_topic(bool top, Client cl, Channel& chan) {
	if (chan.getTopicMode() && !(cl.Get_state() & OPER)) {Reply::ERR_CHANOPRIVSNEEDED(cl, chan.getName()); return ;}
	else chan.setTopicMode(top);
}

const bool	OnChannel(Client client, std::string channel) {
	for (std::vector<Channel>::iterator it = client.Get_chan().begin(); it != client.Get_chan().end(); it++)
		if (it->getName() == channel) return true;
	return false;
}

void ChangePassword(Client cl, Channel& chan) {
	if (!(cl.Get_state() & OPER)) {Reply::ERR_CHANOPRIVSNEEDED(cl, chan.getName()); return ;}
	if (chan.getKeyMode()) {}
}

void	Server::cMODE(std::vector<std::string> messages, int fd) {
	std::vector<Client>::iterator cl = getClientit(fd);
	std::vector<std::string>::iterator msg;

	if (!(cl->Get_state() & REG)) {Reply::ERR_NOTREGISTERED(*cl); return ;}


	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else {Reply::ERR_NEEDMOREPARAMS(*cl, messages[0]); return ;}

	//for user MODE//look out for this command in channel //or not idk
	try {
		Client nick = getClientName(*msg);
		if (nick.Get_nick() != cl->Get_nick()) {Reply::ERR_USERSDONTMATCH(*cl); return ;}
		
		if (messages.size() == 2) { Reply::RPL_UMODEIS(*cl); return ;}
		
		if (!Check_mod_ops(msg + 1, messages.end(), cl)) return ;

		while (++msg != messages.end()) {
			if (msg->at(1) != 'i') return ;

			if (msg->at(0) == '+' && !(cl->Get_state() & INV)) cl->Set_state(INV);
			if (msg->at(0) == '-' && (cl->Get_state() & INV)) cl->Set_state(-INV);
		}
		return ;
	}
	catch (std::exception &err){}


//	for channel MODE
	try {
		std::vector<Channel>::iterator chan = getChanName(*msg);
		if (OnChannel(*cl, chan->getName())) {Reply::ERR_NOTONCHANNEL(*cl, chan->getName()); return ;}
		//if (!(cl->Get_state() & OPER)) {Reply::ERR_CHANOPRIVSNEEDED(*cl, chan->getName()); return ;}depends on the modifications asked

		if (!Check_mod_ops(msg, messages.end(), cl)) return ;

		while (++msg != messages.end()) {
			short unsigned int mod = msg->at(1);

			switch (mod) {
				case ('i') :
					msg->at(0) == '+' ? chan->setInviteo(true) : chan->setInviteo(false);
				case ('t') :
					msg->at(0) == '+' ? Change_topic(true, *cl, *chan) : Change_topic(false, *cl, *chan);
				case ('k') :
				{
					msg->at(0) == '+' ? ChangePassword() : ChangePassword();

				}
				case ('o') :
					msg->at(0) == '+' ? : ;
				case ('l') :
					msg->at(0) == '+' ? : ;
			}
			
		}
	}
	catch (std::exception &err){}

	//if (*msg == cl->Get_nick()) Reply::ERR_USERSDONTMATCH(*cl);

}