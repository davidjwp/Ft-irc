#include "irc.hpp"

Channel::Channel(){}

Channel::Channel(const std::string& name, Client client): 
_name(name), _limit(-1), _pass(""), _topic(""), _invit_mode(false), _topic_mode(false), _key_mode(false) , _op_mode(false){
	_clients[client.Get_nick()] = client;
	_operators[client.Get_nick()] = client;
}

const std::string Channel::getName() const {return _name;}
bool Channel::getInvit() const {return _invit_mode;}
int Channel::getLimit() const {return _limit;}
const std::string Channel::getPass() const {return _pass;}
const std::string Channel::getTopic() const {return _topic;}
const std::map<const std::string, Client>::iterator Channel::getClientsIt() { return _clients.begin();}
std::map<const std::string, Client>& Channel::getClient() {return _clients;}
const std::map<const std::string, Client>::iterator Channel::getOperators() { return _operators.begin();}
bool Channel::getInvitMode() const {return _invit_mode;}
bool Channel::getTopicMode() const {return _topic_mode;}
bool Channel::getKeyMode() const {return _key_mode;}
bool Channel::getOpMode() const {return _op_mode;}

void Channel::addOper(Client& client) {_operators[client.Get_nick()] = client;}
void Channel::setOperMode(bool mode) {_op_mode = mode;}
void Channel::remOper(Client& client) {_operators.erase(client.Get_nick());}
void Channel::AddClient(Client& client) {_clients[client.Get_nick()] = client;}
void Channel::setName(std::string name) {_name = name;}
void Channel::setInviteo(bool inv) {_invit_mode = inv;}
void Channel::setLimit(int limit) {_limit = limit;}
void Channel::setPass(std::string pass) {_pass = pass;}
void Channel::setTopic(std::string topic) {_topic = topic;}
void Channel::setTopicMode(bool top) {_topic_mode = top;}
void Channel::setKeyMode(bool key) {_key_mode = key;}

bool Channel::IsBanned(const std::string& client){
	for (std::vector<std::string>::iterator it = _banned.begin(); it != _banned.end() ;it++)
		if (client == *it) return true;
	return false;
}

bool Channel::IsOperator(const Client& cl) const {
	for (std::map<const std::string, Client>::const_iterator it = _operators.begin(); it != _operators.end(); it++)
		if (it->second.Get_nick() == cl.Get_nick()) return true;
	return false;
}

void Channel::Broadcast(std::string msg, int fd) const {
	msg += "\r\n";
	for (std::map<const std::string, Client>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
		if (fd != it->second.Get_clfd() && send(it->second.Get_clfd(), msg.c_str(), msg.size(), 0) == -1) throw Error("Error: Channel::Broadcast send failed.");
}

void Channel::Broadcast(std::string msg) const {
	msg += "\r\n";
	for (std::map<const std::string, Client>::const_iterator it = _clients.begin(); it != _clients.end(); it++)
		if (send(it->second.Get_clfd(), msg.c_str(), msg.size(), 0) == -1) throw Error("Error: Channel::Broadcast send failed.");
}

void Channel::EraseClient(Client& cl) {
	for(std::map<const std::string, Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		if ((it->second.Get_clfd() == cl.Get_clfd())) {
			std::cout << colstring(Byellow, std::string("Erasing client ...")) << std::endl;
			Broadcast(cl.makeCLname() + " PART :" + _name);
			_clients.erase(it);
			remOper(cl);
			return ;
		}
	}
	std::cout << colstring(Byellow, std::string("not really erasing client")) << _clients.size() << std::endl;
}
