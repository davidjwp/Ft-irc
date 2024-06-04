#include "irc.hpp"

Server::~Server(){}

Server::Server(int port, const std::string &pass): _host(std::string("127.0.0.1")), _pass(pass), _opass("b_operator"), _port(port) {
	// CREATING A SOCKET
	if ((_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) throw Error("Error: Server::Server socket creation failed.");
	
	int opt = 1;
	if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) throw Error("Error: Server::Server socket option failed.");
	
	if (fcntl(_sock, F_SETFL, O_NONBLOCK) < 0) throw Error("Error: Server::Server error changing socket to non blocking");

	// BIND THE SOCKET TO A IP / PORT AND LISTEN
	sockaddr_in sockadd;
	bzero(&sockadd, sizeof(sockadd));
	sockadd.sin_family = AF_INET;//address family of socket
	sockadd.sin_addr.s_addr = INADDR_ANY;
	sockadd.sin_port = htons(_port);//correct byte order on port

	if (bind(_sock, (sockaddr*)&sockadd, sizeof(sockadd)) < 0) throw Error("Error: Server::Server socket bind failed.");
	
	if (listen(_sock, SOMAXCONN) < 0) throw Error("Error: Server::Server error while listening.");
}

int	Server::Start_server(){
	//MULTIPLEXING THE SERVER PORT
	pollfd serverfd = {_sock, POLLIN, 0};
	_pollfds.push_back(serverfd);

	std::cout << colstring(Bblue, std::string("IRC ")) << "server starting !" << std::endl;
	while (!stop_server)
	{
		if (poll(_pollfds.data(), _pollfds.size(), -1) < 0)
			break ;
		for (size_t i = 0; i < _pollfds.size(); i++) {
			if (!_pollfds[i].revents) continue;
			if ((_pollfds[i].revents  & POLLIN ) == POLLIN) {
				if (_pollfds[i].fd == _sock) {
					Add_client();
					break;
				}
			}
			Client_messages(_pollfds[i].fd);
			Clients_Status();
		}
	}
	for (size_t i = 0; i < _pollfds.size(); i++)
		close(_pollfds[i].fd);
	return 0;
}

void	Server::Add_client(){
	//NEW CLIENT
	int	clientsocket;
	sockaddr_in client = {};
	socklen_t clientSize = sizeof(client);
	char hostname[NI_MAXHOST];

	if ((clientsocket = accept(_sock, (sockaddr*)&client, &clientSize)) < 0) throw Error("Error: Server::Add_client accept failed.");
	
	if (getnameinfo((sockaddr*)&client, sizeof(client), hostname, NI_MAXHOST, NULL, NI_MAXSERV, 0)) throw Error("Error: Server::Add_client getnameinfo failed.");
	
	_clients.push_back(Client(clientsocket, hostname));
	pollfd clpfd = {clientsocket, POLLIN, 0};
	_pollfds.push_back(clpfd);

	Clients_Status();
}

void	Server::Clients_Status() {
	std::cout << colstring(Bcyan, std::string("Connected clients: ")) << _clients.size() << std::endl;

	for (size_t i = 0; i < _clients.size(); i++) {
		int state = _clients[i].Get_state();
		std::cout << (i + 1) << ".\tregistered: [";
		if (state & NICK && state & USER && state & PASS)
			std::cout << colstring(Bgreen, std::string("yes"));
		else
			std::cout << colstring(Bred, std::string("no"));
		std::cout << 
		"]\tuser:" << colstring(Bblue, _clients[i].Get_user()) <<
		"\tnickname:" << colstring(Bblue, _clients[i].Get_nick()) <<
		"\treal name:" << colstring(Bblue, _clients[i].Get_realname()) <<
		std::endl;
	}
}

//revised version
//std::string	Server::Get_message(int clfd) {
//	std::string	msg;
//	char	buf[BUFFER_SIZE];
//	bzero(buf, BUFFER_SIZE);
//	std::vector<Client>::iterator client = getClientit(clfd);
//	msg = client->Get_msg();

//	while (!std::memchr(buf, '\n', BUFFER_SIZE)){
//		int n = recv(clfd, buf, BUFFER_SIZE, MSG_DONTWAIT);
//		if (n < 0) {
//			if (errno != EWOULDBLOCK) throw Error("Error: Server::Get_message recv blocking error.");
//			return "";
//		} else if (n == 0) {
//			throw Error("");
//		}
//		client->addMsg(std::string(buf, n));
//		msg.append(buf, n);
//		if (std::memchr(buf, '\n', n)) break;
//		msg += buf;
//	}
//	client->Set_msg("");
//	return msg;
//}

std::string Server::Get_message(int clfd) {
    std::string msg;
    char buf[BUFFER_SIZE];
    bzero(buf, BUFFER_SIZE);
    std::vector<Client>::iterator client = getClientit(clfd);
    msg = client->Get_msg();

    while (true) {
        int n = recv(clfd, buf, BUFFER_SIZE, 0); // No need for MSG_DONTWAIT since poll() ensures readiness
        if (n < 0) {
            throw Error("Error: Server::Get_message recv error.");
        } else if (n == 0) {
            throw Error("Error: Client disconnected.");
        }
        client->addMsg(std::string(buf, n));
        msg.append(buf, n);
        if (std::memchr(buf, '\n', n)) break;
    }
    client->Set_msg("");
    return msg;
}

//std::string	Server::Get_message(int clfd) {
//	std::string	msg;
//	char	buf[256];
//	bzero(buf, 256);
//	std::vector<Client>::iterator client = getClientit(clfd);
//	msg = client->Get_msg();

//	while (!std::strstr(buf, "\n")){
//		bzero(buf,256);
//		int n = 0;
//		if ((n = recv(clfd, buf, 256, MSG_DONTWAIT)) < 0){
//			if (errno != EWOULDBLOCK) throw Error("Error: Server::Get_message recv blocking error.");
//			return "";
//		}
//		else if (n == 0) throw Error("Error: Server::Get_message client disconnect.");
//		client->addMsg(buf);
//		msg += buf;
//	}
//	client->Set_msg("");
//	return msg;
//}

std::vector<std::string> Server::split(std::string msg) {
	std::vector<std::string> cmd;
	std::stringstream str(msg);
	std::string tmp;
	int i = 0;
	if (msg == "\n")
		return cmd;
	while (std::getline(str, tmp, '\n')) 
	{
		cmd.push_back(tmp);
		std::cout << cmd.at(i++) << std::endl;//this is where the output comes from
	}
	return cmd;
}

void	Server::Disconnect_client(int clfd) {
	Client cl = GetClient(clfd);

	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++) {
		it->EraseClient(cl);
		if (it->getClient().empty())
			it = _channels.erase(it);
		else
			it++;
	}
	std::cout << colstring(Byellow, std::string("Client erased from channels")) << std::endl;

	_clients.erase(getClientit(clfd));
	_pollfds.erase(getPollfd(clfd));

	std::cout << colstring(Bcyan, std::string("User: ")) << 
	colstring(Bblue, cl.Get_user()) << 
	colstring(Bcyan, std::string(" Successfully Disconnected !")) << std::endl;

	close(clfd);
}

//std::vector<std::string>

void	Server::Client_messages(int current_clfd) {
	std::vector<std::string> messages;

	try{ messages = split(Get_message(current_clfd)); }
	
	catch (std::exception& err){
		Disconnect_client(current_clfd);
		std::cerr << err.what() << std::endl;
		return ;
	}
	for (size_t i = 0; i < messages.size(); i++)
		Proc_message(messages[i], current_clfd);
}

//Channel specific commands 
//JOIN, OPER, KICK, MODE, TOPIC, NAMES, OPER, PART, INVITE

//User specific commands
//NICK, USER, PASS, PING
void	Server::Proc_message(std::string message, int clfd) {

	std::vector<std::string> msg_split;

	if (message[0] == ' ' || message[0] == '\t') return ;
		
	while (message.find('\t') != std::string::npos)
		message[message.find('\t')] = 32;

	std::string 	tmp;
	std::stringstream stream(message);
	while (getline(stream, tmp, ' '))
		msg_split.push_back(tmp);

	std::string  ccoms[] = {"NICK", "USER", "PASS", "MODE", "JOIN", "PRIVMSG", "OPER", \
							"PART", "NAMES", "KICK", "INVITE", /*"TOPIC",*/ "PING"};
	
	void	(Server::*commands[])(std::vector<std::string> msg_split, int clfd) = {
		&Server::cNICK, &Server::cUSER, &Server::cPASS, &Server::cMODE, &Server::cJOIN, &Server::cPRIVMSG, 
		&Server::cOPER, &Server::cPART, &Server::cNAMES, &Server::cKICK, 
		&Server::cINVITE, /*&Server::cTOPIC,*/ &Server::cPING};

	for (int i = 0; i < 14; i++){
		if (!msg_split[0].compare(ccoms[i])) {
			(this->*commands[i])(msg_split, clfd); 
			break;
		}
	}
}

Client& Server::GetClient(int fd) {
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (it->Get_clfd() == fd) return *it;
	throw Error("Error: Server::GetClient can't find client fd in server _clients.");
	return _clients.at(0);
}

std::vector<pollfd>::iterator Server::getPollfd(int fd){
	for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); it++)
		if (it->fd == fd) return it;
	throw Error("Error: Server::getPollfd can't find pollfd.");
	return _pollfds.begin();
}

std::vector<Client>::iterator Server::getClientit(int fd){
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (it->Get_clfd() == fd) return it;
	throw Error("Error: Server::getClient can't find client.");
	return _clients.begin();
}

Client& Server::getClientName(const std::string& nickname){
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++)
		if (it->Get_nick() == nickname) return *it;
	throw Error("Error: Server::getClientName can't find client.");
	return _clients.at(0);
}

/*
	takes a string and outputs a channel iterator corresponding to the string given.
	if not found throws an exception.
*/
std::vector<Channel>::iterator Server::getChanName(const std::string& chan){
	if (!_channels.size()) throw Error ("Error: Server::getChanName no channel.");
	for (std::vector<Channel>::iterator it = _channels.begin(); it != _channels.end(); it++)
		if (it->getName() == chan) return it;
	throw Error ("Error: Server::getChanName can't find channel.");
}

void Server::AddChannel(Channel& chan) {_channels.push_back(chan);}

//GETTERS

std::string Server::Get_host(){ return _host;}
std::string Server::Get_pass(){ return _pass;}
std::string Server::Get_opass(){ return _opass;}


//IMPLEMENT PING TO PREVENT IRSSI FROM RECONNECTING 


//watch out for channel names, nicknames, username and all changes to clients in the PROTOCOL

/*
### Common Privileged Channel Modes

1. **Invite-Only (`+i`)**:
   - **Who Can Set It**: Only channel operators or higher can set this mode.
   - **Command**: `/mode #channel +i`
   
2. **Moderated (`+m`)**:
   - **Who Can Set It**: Only channel operators or higher can set this mode.
   - **Command**: `/mode #channel +m`
   
3. **Topic Settable by Ops Only (`+t`)**:
   - **Who Can Set It**: Only channel operators or higher can set this mode.
   - **Command**: `/mode #channel +t`
   
4. **Key (Password) Protected (`+k`)**:
   - **Who Can Set It**: Only channel operators or higher can set this mode.
   - **Command**: `/mode #channel +k <password>`
   
5. **User Limit (`+l`)**:
   - **Who Can Set It**: Only channel operators or higher can set this mode.
   - **Command**: `/mode #channel +l <number>`

### Example

If a user named `john` is a channel operator in the `#example` channel, he can set the channel to invite-only mode with the following command:

```
/mode #example +i
```

If `john` is not a channel operator and attempts to set the invite-only mode, he will receive an error, typically `ERR_CHANOPRIVSNEEDED` (482):

```
:irc.example.com 482 john #example :You're not channel operator
```

### Permissions and Roles

- **Channel Founder (`+q`)**: The highest level of control, typically can do anything within the channel.
- **Channel Protected (`+a`)**: Protected status, often cannot be kicked by regular operators.
- **Channel Operator (`+o`)**: Standard operator, can change channel modes, kick users, etc.
- **Channel Half-Operator (`+h`)**: Limited operator, can perform some but not all operator functions.
- **Voice (`+v`)**: Can speak in moderated channels but does not have operator privileges.
*/