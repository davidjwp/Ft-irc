#include "irc.hpp"

//EXCEPTION
class Error: public std::exception{
private:
	std::string	_msg;

public:
	virtual ~Error() throw (){};
	Error(const char* msg): _msg(std::string(msg)){};
	const char* what() const throw(){ return _msg.c_str();}
};

//CLIENT IMPLEMENTATION#########################################################################################################

Client::Client(int clientfd, const std::string hostname): _hostname(hostname), _clfd(clientfd), _state(0){}

Client::~Client(){}

//GETTERS

std::string Client::Get_host(){ return _hostname;}
std::string Client::Get_nick(){ return _nickname;}
std::string Client::Get_chan(){ return _channel;}
std::string Client::Get_user(){ return _username;}
std::string	Client::Get_msg(){ return _msg;}
std::string Client::Get_realname(){ return _realname;}
int	Client::Get_state(){ return _state;}
int	Client::Get_clfd(){ return _clfd;}

//SERVER IMPLEMENTATIONS#########################################################################################################
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

	//std::cout << "Server IRC Launched!" << std::endl;
	while (!stop_server)
	{
		if (poll(_pollfds.data(), _pollfds.size(), -1) < 0)
			break ;
		for (size_t i = 0; i < _pollfds.size(); i++) {
			if (_pollfds[i].revents && (_pollfds[i].revents  & POLLIN ) == POLLIN) {
				if (_pollfds[i].fd == _sock) {
					Add_client();
					break;
				}
			}
			Client_messages(_pollfds[i].fd);
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

	Client_Status();
}

void	Server::Client_Status() {
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

//int	gnl(char **line)
//{
	//char	*buf;
	//int		n;
	//char	c;
	//int		i;
//
	//i = 0;
	//write (1, "heredoc> ", 9);
	//buf = malloc(sizeof(char) * BUFSIZ);
	//n = read(STDIN_FILENO, &c, 1);
	//while (n && c != '\n' && c != '\0')
	//{
		//if (c != 'n' && c != '\0')
			//buf[i] = c;
		//n = read(STDIN_FILENO, &c, 1);
		//i++;
	//}
	//buf[i] = '\n';
	//buf[++i] = 0;
	//*line = buf;
	//return (n);
//}


std::string	Server::Get_message(int clfd) {
	std::string	msg;
	char	buf[BUFFER_SIZE];
	bzero(buf, BUFFER_SIZE);

	while (!std::memchr(buf, '\n', BUFFER_SIZE)){
		bzero(buf,BUFFER_SIZE);
		if (recv(clfd, buf, BUFFER_SIZE, MSG_DONTWAIT) < 0){
			if )
		}
	}

	return msg;
}

//std::string	Server::Get_message(int clfd) {
//	std::string	msg;
//	char	buf[256];
//	bzero(buf, 256);

//	while (msg.find('\n') == std::string::npos){
//		bzero(buf,256);
//		if (recv(clfd, buf, 256, MSG_DONTWAIT))
//	}

//	return msg;
//}

void	Server::Client_messages(int current_clfd) {
	try{
		std::string message = Get_message(current_clfd);
		std::vector<std::string> messages;
	}
	catch (std::exception& err){
		std::cerr << err.what() << std::endl;
	}
}

Client Server::getClient(int fd){
	for (size_t i = 0; i < _clients.size(); i++) {
		if (_clients[i].Get_clfd() == fd) return _clients[i];
		i++;
	}
	throw Error("Error: Server::getClient can't find client.");
	return *_clients.data();
}

//GETTERS

std::string Server::Get_host(){ return _host;}
std::string Server::Get_pass(){ return _pass;}
std::string Server::Get_opass(){ return _opass;}


//MAIN##########################################################################################################################


void	Signal_handler(const int signal) {
	(void)signal;
	stop_server = true;
	//std::cout << "\b\b";
}

int	Check_port(char* arg) {
	char *endptr;
	long int port = strtol(arg, &endptr, 10);
	if (*endptr != '\0') throw Error("Error: Check_port only digits in port allowed.");
	if (port > 65535 || port < 0) throw Error("Error: Check_port wrong port range.");
	return port;
}

int	main(int ac, char **av) {
	stop_server = false;
	signal(SIGINT, Signal_handler);

	if (ac != 3) { std::cout << "wrong arguments!\n" << "<program> <port>  <password>" << std::endl; return 1;}
	try {
		Server srv(Check_port(av[1]), av[2]);
		srv.Start_server();
	}
	catch (std::exception& err) {//this could lead to still open fds 
		std::cerr << err.what() << std::endl;
		return -1;
	}

	return 0;
}

	//	while receiving- display message, echo message// this is for the purpose of connection not irc
	//char buf[4096];
	//while (true)
	//{
	//	//clear buffer
	//	memset(buf, 0, 4096);
	//	//wait for message 
	//	int bytesRecv = recv(clientSocket, buf, 4096, 0);
	//	if (bytesRecv == -1)
	//	{
	//		std::cerr << "there was a connection issue" << std::endl;
	//		break ;
	//	}
	//	if (bytesRecv == 0)
	//	{
	//		std::cout << "the client disconnected" << std::endl;
	//		break ;
	//	}
	//	//display message
	//	std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
	//	//send message
	//	send(clientSocket, buf, bytesRecv + 1, 0);
	//}
	////	Close socket
	//close(clientSocket);
