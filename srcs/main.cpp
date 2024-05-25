#include <sys/types.h>
//#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include <algorithm>
#include <signal.h>
#include <vector>
#include <exception>
#include <fcntl.h>

//Cheat sheet :
//Socket: Create a new communication
//Bind: Attach a local address to a socket
//Listen: Announce willingness to accept connections
//Accept: Block caller until a connection request arrives
//Connect: Actively attempt to establish a connection
//Send: Send some data over a connection
//Receive: Receive some data over a connection
//Close: Release the connection

//	steps of IRC server 
//	
//1.	create a socket
//	
//2.	bind the socket to an IP address and protected
//	
//3.	mark the socket for listening 
//	
//4.	accept incoming connections
//	
//5.	handle multiple client connections
//	
//6.	receive and process LC_MESSAGES
//	
//7.	close client socket
//	
//8.	close the listening port

bool	g_signal;

class Server {
private:
	std::string	_host;
	std::string _nick;
	std::string _name;
	std::string _chan;
	std::string _pass;
	std::string	_opass;

	int	_port;
	int	_sock;

	std::vector<pollfd> _pollfds;

	Server();
	Server(Server&);
	Server& operator=(Server&);
public:
	Server(int port, const std::string &pass);
	Server(std::string port, std::string pass);
	~Server();

	int Start_server();
};

class Client {
private:


	std::string	_hostname;
	std::string	_nickname;
	std::string	_channel;
	std::string	_username;

};

//EXCEPTION
class Error: public std::exception{
private:
	std::string	_msg;
public:
	Error(char* msg): _msg(msg){}
	const char* what() throw() {return _msg.c_str();}
};


void	Signal_handler(const int signal) {
	(void)signal;
	g_signal = true;
	std::cout << "\b\b";
}

int	Check_port(char* arg) {
	char *endptr;
	long int port = strtol(arg, &endptr, 10);
	if (*endptr != '\0') throw Error("Error: Check_port only digits in port allowed.");
	if (port > 65535 || port < 0) throw Error("Error: Check_port wrong port range.");
	return port;
}

Server::~Server(){}

Server::Server(int port, const std::string &pass): _port(port), _pass(pass), _host(std::string("127.0.0.1")), _opass("b_operator") {
	// CREATING A SOCKET
	if (_sock = socket(AF_INET, SOCK_STREAM, 0) < 0) throw Error("Error: Server::Server socket creation failed.");
	int opt;
	if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &opt, (socklen_t)opt) < 0) throw Error("Error: Server::Server socket option failed.");
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
	//SET POLL
	pollfd fd_server = {_sock, POLLIN, 0};
	_pollfds.push_back(fd_server);
	
	std::cout << "Server IRC Launched!" << std::endl;
	while (!g_signal)
	{
		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
			break ;
		for (unsigned int i = 0; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].revents == 0)
				continue ;
			if ((_pollfds[i].revents  & POLLIN ) == POLLIN)
			{
				if (_pollfds[i].fd == _sock)
				{
					//newClient();
					//displayClient();
					break;
				}
			}
			//handleMessage(_pollfds[i].fd);
		}
	}
	for (size_t i = 0; i < _pollfds.size(); i++)
		close(_pollfds[i].fd);

}

int	main(int ac, char **av) {
	g_signal = false;
	signal(SIGINT, Signal_handler);

	if (ac != 3) { std::cout << "wrong arguments!\n" << "<program> <port>  <password>" << std::endl; return 1;}
	try {
		Server srv(Check_port(av[1]), av[2]);
		srv.Start_server();
	}
	catch (std::exception& err) {
		std::cerr << err.what() << std::endl;
		return -1;
	}

	///+++ before accepting a call make a pollfd struct for multiplexing  
	///+++ then use poll() which will wait for one of the file descriptors to become ready to perform I/O
	///+++ this step is for irc since irc listens to multiple fds through the same port 
	///+++ the rest of this main is when you create a new client, you found a polld trying to connect(through an event)
	///+++ so you accept the call and create a new client 
	//pollfd fd_server = {_sock, POLLIN, 0};
	//std::vector<pollfd> _pollfds;
	//_pollfds.push_back(fd_server);
	/////+++this part is in a loop 
	//while (g_signal == false)
	//{
	//	if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
	//		break ; 
	//	for (unsigned int i = 0; i < _pollfds.size(); i++)
	//	{
	//		if (_pollfds[i].revents == 0)
	//			continue ;
	//		if ((_pollfds[i].revents  & POLLIN ) == POLLIN)
	//		{
	//			if (_pollfds[i].fd == _sock)
	//			{
	//				//newClient(); CREATE NEW CLIENT THIS IS THE CONTINUATION OF THIS PROGRAM
	//				//displayClient();
	//				break;
	//			}
	//		}
	//		//handleMessage(_pollfds[i].fd);

	//	}
	//}
	///+++
	//	accept a call
	//NEW CLIENT 
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];//max value of maxlen// host name
	char service[NI_MAXSERV];//max value for servlen// port the client is on

	int clientSocket = accept(_sock, (sockaddr*)&client, &clientSize);//accept connection to socket 
	if (clientSocket == -1){
		std::cerr << "Error with accepting new client" << std::endl;
		return -4;
	}

	//	Close the listening socket// in irc multiple connexions go through the same port so you don't close it, this is for example purposes
	//close(listening);//not in irc// close listening port since we don't need it  

	memset(host, 0, NI_MAXHOST);
	memset(service, 0 , NI_MAXSERV);

	int result = getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);//
	if (result)
	{
		std::cout << host << " connected on " << service << std::endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		std::cout << host << " connected on " << ntohs(client.sin_port) << std::endl;
	}
	//	while receiving- display message, echo message// this is for the purpose of connectio not irc
	char buf[4096];
	while (true)
	{
		//clear buffer
		memset(buf, 0, 4096);
		//wait for message 
		int bytesRecv = recv(clientSocket, buf, 4096, 0);
		if (bytesRecv == -1)
		{
			std::cerr << "there was a connection issue" << std::endl;
			break ;
		}
		if (bytesRecv == 0)
		{
			std::cout << "the client disconnected" << std::endl;
			break ;
		}
		//display message
		std::cout << "Received: " << std::string(buf, 0, bytesRecv) << std::endl;
		//send message
		send(clientSocket, buf, bytesRecv + 1, 0);
	}
	//	Close socket
	close(clientSocket);
	return 0;
}


//for (unsigned int i = 0; i < _pollfds.size(); i++):

//Iterate over all the file descriptors in _pollfds.
//if (_pollfds[i].revents == 0) continue;:

//If there are no events (revents is zero), skip to the next file descriptor.
//if ((_pollfds[i].revents & POLLIN) == POLLIN):

//Check if the POLLIN event is set, meaning the file descriptor is ready for reading.
//if (_pollfds[i].fd == _sock):

//If the file descriptor is the server's listening socket (_sock), it means a new client is trying to connect.
//newClient();:
//This function likely accepts the new client connection and adds the new client's file descriptor to _pollfds.
//displayClient();:
//This function likely outputs information about the newly connected client.
//break;:
//Break out of the loop to restart polling after handling the new client connection.

