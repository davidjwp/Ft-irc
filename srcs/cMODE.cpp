#include "irc.hpp"

//WORK IN PROGRESS

//void	Reply::ERR_UNKNOWNMODE(const Client& cl){}

static bool Check_mod_ops(
	std::vector<std::string>::iterator msgs, 
	std::vector<std::string>::iterator end,
	std::vector<Client>::iterator cl) {
	while (msgs != end) {
		msgs->erase(msgs->find('\r'));

		if (msgs->size() != 2) {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return false;}

		if (msgs->at(0) != '-' && msgs->at(0) != '+') {Reply::ERR_UMODEUNKNOWNFLAG(*cl); return false;}

		if (msgs->at(1) != 'i' && msgs->at(1) != 'k' && msgs->at(1) != 'l' && msgs->at(1) != 'o' && msgs->at(1) != 't')
		{Reply::ERR_UMODEUNKNOWNFLAG(*cl); return false;}
		msgs++;
	}
	return true;
}

bool	Server::OnChannel(Client& client, Channel& chan) const{
	for (std::map<const std::string, Client>::iterator it = chan.getClient().begin(); it != chan.getClient().end(); it++)
		if (it->second.Get_nick() == client.Get_nick())


	
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
		newkey.erase(newkey.find('\r'));
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
	msg.erase(msg.find('\r'));
	char *buff;
	size_t limit = strtol(msg.c_str(), &buff, 10);
	if (*buff != '\0')
		limit = 0;
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
		msg->erase(msg->find('\r'));
		std::vector<Channel>::iterator chan = getChanName(*msg);
		if (OnChannel(*cl, chan->getName())) {Reply::ERR_NOTONCHANNEL(*cl, chan->getName()); return ;}
		if (chan->getOpMode() && chan->IsOperator(*cl)) {Reply::ERR_CHANOPRIVSNEEDED(*cl, chan->getName()); return ;}

		if (msg + 1 == messages.end()) Reply::RPL_CHANNELMODEIS(*cl, *chan);
		if (!Check_mod_ops(msg, messages.end(), cl)) return ;

		while (++msg != messages.end()) {
			short unsigned int mod = msg->at(1);

			switch (mod) {
				case ('i') : {
					msg->at(0) == '+' ? ChangeInvito(true, *cl, *chan) : ChangeInvito(false, *cl, *chan);
					break ;	
				}
				case ('t') : {
					msg->at(0) == '+' ? Change_topic(true, *cl, *chan) : Change_topic(false, *cl, *chan);
					break ;
				}
				case ('k') : {
					if (msg + 1 == messages.end()) {Reply::ERR_NEEDMOREPARAMS(*cl, *msg); break ;}
					msg->at(0) == '+' ? ChangePassword(true, *(msg + 1), *cl, *chan) : ChangePassword(false, *(msg + 1), *cl, *chan);
					break ;
				}
				case ('o') : {
					if (msg + 1 == messages.end()) {Reply::ERR_NEEDMOREPARAMS(*cl, *msg); break ;}
					msg->at(0) == '+' ? ChangeOper(true, *(msg + 1), *chan, *cl) : ChangeOper(false, *(msg + 1), *chan, *cl);
					break ;
				}
				case ('l') : {
					if (msg + 1 == messages.end()) {Reply::ERR_NEEDMOREPARAMS(*cl, *msg); break ;}
					msg->at(0) == '+' ? ChangeLimit(true, *(msg + 1), *chan, *cl): ChangeLimit(false, *(msg + 1), *chan, *cl);
					break ;
				}
				default : {Reply::ERR_UMODEUNKNOWNFLAG(*cl); break ;}
			}
		}
	}
	catch (std::exception &err){}
	//if (*msg == cl->Get_nick()) Reply::ERR_USERSDONTMATCH(*cl);
}

/*In IRC (Internet Relay Chat), the `MODE` command is used to change the modes of a user or a channel. Each type of mode change can result in specific replies (RPLs) or error messages (ERRs). Below is a list of some of the common replies and error messages associated with the `MODE` command according to the IRC protocol:

### Replies for Successful MODE Changes (RPLs)

1. **RPL_CHANNELMODEIS (324)**:
   - **Description**: Sent to a user to inform them of the current channel modes.
   - **Format**:
     ```plaintext
     :<server> 324 <nickname> <channel> <modes> <mode_params>
     ```
   - **Example**:
     ```plaintext
     :irc.example.com 324 john #example +nt
     ```

2. **RPL_CREATIONTIME (329)**:
   - **Description**: Provides the creation time of the channel.
   - **Format**:
     ```plaintext
     :<server> 329 <nickname> <channel> <timestamp>
     ```
   - **Example**:
     ```plaintext
     :irc.example.com 329 john #example 1616161616
     ```

3. **RPL_UMODEIS (221)**:
   - **Description**: Sent to a user to inform them of their current user modes.
   - **Format**:
     ```plaintext
     :<server> 221 <nickname> <usermodes>
     ```
   - **Example**:
     ```plaintext
     :irc.example.com 221 john +i
     ```

### Error Replies for MODE Command (ERRs)

1. **ERR_NEEDMOREPARAMS (461)**:
   - **Description**: Returned when a client tries to change a mode but does not provide enough parameters.
   - **Format**:
     ```plaintext
     :<server> 461 <nickname> <command> :Not enough parameters
     ```
   - **Example**:
     ```plaintext
     :irc.example.com 461 john MODE :Not enough parameters
     ```

2. **ERR_KEYSET (467)**:
   - **Description**: Returned when a client tries to set a key for a channel that already has one.
   - **Format**:
     ```plaintext
     :<server> 467 <nickname> <channel> :Channel key already set
     ```
   - **Example**:
     ```plaintext
     :irc.example.com 467 john #example :Channel key already set
     ```

3. **ERR_NOCHANMODES (477)**:
   - **Description**: Returned when a client tries to set a mode on a channel that does not support modes.
   - **Format**:
     ```plaintext
     :<server> 477 <nickname> <channel> :Channel doesn't support modes
     ```
   - **Example**:
     ```plaintext
     :irc.example.com 477 john #example :Channel doesn't support modes
     ```

4. **ERR_USERNOTINCHANNEL (441)**:
   - **Description**: Returned when a client tries to change a mode for a user who is not in the channel.
   - **Format**:
     ```plaintext
     :<server> 441 <nickname> <channel> <user> :They aren't on that channel
     ```
   - **Example**:
     ```plaintext
     :irc.example.com 441 john #example alice :They aren't on that channel
     ```

5. **ERR_NOTONCHANNEL (442)**:
   - **Description**: Returned when a client tries to perform*/