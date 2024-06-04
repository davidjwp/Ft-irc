#include "irc.hpp"

Client::Client(): 
_hostname(""), _nickname(""), _username(""), 
_realname(""), _channels(std::vector<Channel>()), _clfd(0), _state(0), _msg(""){}

Client& Client::operator=( Client& cl){
	_hostname = cl.Get_host();
	_nickname = cl.Get_nick();
	_username = cl.Get_user();
	_realname = cl.Get_realname();
	_channels = cl.Get_chan();
	_clfd = cl.Get_clfd();
	_state = cl.Get_state();
	_msg = cl.Get_msg();
	return *this;
}

Client::Client(int clientfd, const std::string hostname): 
_hostname(hostname), _nickname(""), _username(""), _realname(""), 
_channels(std::vector<Channel>()), _clfd(clientfd), _state(0){}

Client::~Client(){}

//GETTERS

const std::string Client::Get_host() const { return _hostname;}
const std::string Client::Get_nick() const { return _nickname;}
std::vector<Channel>& Client::Get_chan() { return _channels;}
const std::string Client::Get_user() const { return _username;}
const std::string	Client::Get_msg() const { return _msg;}
const std::string Client::Get_realname() const { return _realname;}
int	Client::Get_state() const { return _state;}
int	Client::Get_clfd() const { return _clfd;}

//SETTERS

void Client::Set_host(std::string host){ _hostname = host;}
void Client::Set_nick(std::string nick){ if (!(_state & NICK)) _state += NICK; _nickname = nick;}
void Client::add_chan(Channel& chan){ _channels.push_back(chan);}
void Client::Set_user(std::string user){ if (!(_state & USER)) _state += USER; _username = user;}
void Client::Set_msg(std::string msg){ _msg = msg;}
void Client::Set_realname(std::string realn){ _realname = realn;}
void Client::Set_state(int state){ _state += state;}
void Client::addMsg(std::string msg){ _msg += msg;}

void Client::isRegistered() {
	if (_state & NICK && _state & USER && _state & PASS) {
		if (!(_state & REG)) {
			_state += REG;
			Reply::RPL_WELCOME(*this);
		}
		return ;
	}
	std::cout << "Registering ... \n" << colstring(Bred, std::string("Missing "));
	if (!(_state & NICK))
		std::cout << "Nickname ";
	if (!(_state & USER))
		std::cout << "Username ";
	if (!(_state & PASS))
		std::cout << "Password ";
	std::cout << std::endl;
}


const std::string Client::makeCLname() const {
	std::string name(":");

	if (_state & USER) name += _nickname;
	if (_state & NICK) name += "!" + _username;
	return name += "@localhost";
}

void Client::reply(const std::string& msg) const {
	std::string fpacket(msg);
	fpacket.insert(0, makeCLname());
	fpacket += "\r\n";
	if (send(_clfd, fpacket.c_str(), fpacket.size(), 0) == -1) throw Error("Error: Client::reply send failed.");
	std::cout << colstring(Bgreen, fpacket) << std::endl; 
}
