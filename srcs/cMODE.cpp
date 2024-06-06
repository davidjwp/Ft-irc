#include "../includes/irc.hpp"

//TO TEST

static bool Check_mod_ops(
	std::vector<std::string>::iterator msgs, 
	std::vector<Client>::iterator cl) {
	if (msgs->find('\r') != std::string::npos) msgs->erase(msgs->find('\r'));

	if (msgs->size() != 2) {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return false;}

	if (msgs->at(0) != '-' && msgs->at(0) != '+') {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return false;}

	if (msgs->at(1) != 'i' && msgs->at(1) != 'k' && msgs->at(1) != 'l' && msgs->at(1) != 'o' && msgs->at(1) != 't')
	{Reply::ERR_UMODEUNKNOWNFLAG(*cl); return false;}
	return true;
}

bool	Server::OnChannel(Client& client, Channel& chan) const{
	for (std::map<const std::string, Client>::iterator it = chan.getClient().begin(); it != chan.getClient().end(); it++)
		if (it->second.Get_nick() == client.Get_nick()) return true;


	
	//for (std::vector<Channel>::iterator it = chan.begin(); it != client.Get_chan().end(); it++)
	//	if (it->getName() == channel) return true;
	return false;
}

void Server::ChangeInvito(bool mode, Client& cl, Channel& chan) const {
	if (chan.getOpMode() && !chan.IsOperator(cl)) {Reply::ERR_CHANOPRIVSNEEDED(cl, chan.getName()); return ;}
	else chan.setInviteo(mode);
}

void  Server::Change_topic(bool top, Client& cl, Channel& chan) const {
	if (chan.getOpMode() && !chan.IsOperator(cl)) {Reply::ERR_CHANOPRIVSNEEDED(cl, chan.getName()); return ;}
	else chan.setTopicMode(top);
}

void Server::ChangePassword(bool mode, std::string newkey, Client& cl, Channel& chan) const{
	if (chan.getOpMode() && !chan.IsOperator(cl)) {Reply::ERR_CHANOPRIVSNEEDED(cl, chan.getName()); return ;}
	chan.setKeyMode(mode);
	if (mode) {
		if (chan.getTopicMode() && chan.IsOperator(cl)) {Reply::ERR_CHANOPRIVSNEEDED(cl, chan.getName()); return ;}
		if (newkey.find('\r') != std::string::npos) newkey.erase(newkey.find('\r'));
		if (!newkey.size()) {Reply::ERR_INVALIDKEY(cl, chan); return ;}
		chan.setPass(newkey);
	}
}

void Server::ChangeOper(bool mode, std::string msg, Channel& chan, Client& cl){
	if (chan.getOpMode() && !chan.IsOperator(cl)) {Reply::ERR_CHANOPRIVSNEEDED(cl, chan.getName()); return ;}
	try {
		Client cl = getClientName(msg);//find client exists using the name
		std::map<const std::string, Client> ChanClients = chan.getClient();
		for (std::map<const std::string, Client>::iterator it = ChanClients.begin(); it != ChanClients.end(); it++) {
			if (it->second.Get_nick() == msg && mode && !(it->second.Get_state() & OPER)) {it->second.Set_state(OPER); return ;}
			else if (it->second.Get_nick() == msg && !mode && it->second.Get_state() & OPER) {it->second.Set_state(-OPER); return ;}
		}
		(void)cl;
	}
	catch (std::exception& err){ Reply::ERR_USERNOTINCHANNEL(cl, msg, chan);//not right numeric
	}
}

void Server::ChangeLimit(bool mode, std::string msg, Channel& chan, Client& cl) const{
	if (chan.getOpMode() && !chan.IsOperator(cl)) {Reply::ERR_CHANOPRIVSNEEDED(cl, chan.getName()); return ;}
	if (msg.find('\r') != std::string::npos) msg.erase(msg.find('\r'));
	int limit;
	if (mode) {
		char *buff;
		limit = strtol(msg.c_str(), &buff, 10);
		if (*buff != '\0')
			limit = 0;
		if (limit < 0) {Reply::ERR_UMODEUNKNOWNFLAG(cl); return ;}
	}
	mode ? chan.setLimit(limit) : chan.setLimit(-1);
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
		if (msg->find('\r') != std::string::npos) msg->erase(msg->find('\r'));
		Client nick = getClientName(*msg);
		if (nick.Get_nick() != cl->Get_nick()) {Reply::ERR_USERSDONTMATCH(*cl); return ;}

		if (messages.size() == 2) { Reply::RPL_UMODEIS(*cl); return ;}

		if (!Check_mod_ops(msg + 1, cl)) return ;

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
		if (msg->find('\r') != std::string::npos) msg->erase(msg->find('\r'));
		std::vector<Channel>::iterator chan = getChanName(*msg);
		if (!OnChannel(*cl, *chan)) {Reply::ERR_NOTONCHANNEL(*cl, chan->getName()); return ;}//HERE
		if (chan->getOpMode() && chan->IsOperator(*cl)) {Reply::ERR_CHANOPRIVSNEEDED(*cl, chan->getName()); return ;}

		if (++msg == messages.end()) {Reply::RPL_CHANNELMODEIS(*cl, *chan); return ;}
		if (!Check_mod_ops(msg, cl)) return ;

		short unsigned int mod = msg->at(1);
		switch (mod) {
			case ('i') : {
				if (msg->at(0) == '+') ChangeInvito(true, *cl, *chan);
				else if (msg->at(0) == '-') ChangeInvito(false, *cl, *chan);
				else {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return ;}
				return ;
			}
			case ('t') : {
				if (msg->at(0) == '+') Change_topic(true, *cl, *chan);
				else if (msg->at(0) == '-') Change_topic(false, *cl, *chan);
				else {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return ;}
				return ;
			}
			case ('k') : {
				if (msg + 1 == messages.end()) {Reply::ERR_NEEDMOREPARAMS(*cl, *msg); return ;}
				if (msg->at(0) == '+') ChangePassword(true, *(msg + 1), *cl, *chan);
				else if (msg->at(0) == '-') ChangePassword(false, *msg, *cl, *chan);
				else {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return ;}
				return ;
			}
			case ('o') : {
				if (msg + 1 == messages.end()) {Reply::ERR_NEEDMOREPARAMS(*cl, *msg); return ;}
				if (msg->at(0) == '+') ChangeOper(true, *(msg + 1), *chan, *cl);
				else if (msg->at(0) == '-') ChangeOper(false, *msg, *chan, *cl);
				else {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return ;}
				return ;
			}
			case ('l') : {
				if (msg->at(0) == '+' && msg + 1 == messages.end()) {Reply::ERR_NEEDMOREPARAMS(*cl, *msg); return ;}
				if (msg->at(0) == '+') ChangeLimit(true, *(msg + 1), *chan, *cl);
				else if (msg->at(0) == '-') ChangeLimit(false, *msg, *chan, *cl);
				else {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return ;}
				return ;
			}
			default : {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return ;}
		}
	}
	catch (std::exception &err){Reply::ERR_NOSUCHCHANNEL(*cl, *msg); return ;}
}