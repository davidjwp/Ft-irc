#include <sys/types.h>
#include <socket.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <vector>
#include <algorithm>

//	steps of IRC server 
//	
//	create a socket
//	
//	bind the socket to an IP address and protected
//	
//	mark the socket for listening 
//	
//	accept incoming connections
//	
//	handle multiple client connections
//	
//	receive and process LC_MESSAGES
//	
//	close client socket
//	
//	close the listening port

class server {
private:

	std::string _nick;
	std::string _name;
	std::string _chan;
	std::string _port;


public:


};

int	main(int ac, char **av){
	if (ac < 3) return 1;
	//	create a socket
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == -1)
	{
		std::cout << "can't create socket" << std::endl;
		return 1;
	}

	sockaddr
	//	bind the socket to a IP / port
	//	Mark the socker for listening in
	//	accept a call
	//	Close the listening socket
	//	while receiving- display message, echo message
	//	Close socket

	return 0;
}
