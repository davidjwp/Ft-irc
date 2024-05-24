#include <sys/types.h>
#include <netinet/in.h>
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
	std::string _port;
	std::string _pass;
	
	Server();
	Server(Server&);
	Server& operator=(Server&);
public:

	Server(std::string port, std::string pass);
	~Server();


};

class Client {
private:


	std::string	_hostname;
	std::string	_nickname;
	std::string	_channel;
	std::string	_username;

};

void	Signal_handler(const int signal){
	(void)signal;
	g_signal = true;
	std::cout << "\b\b";
}

bool	Check_port(char* port){}

int	main(int ac, char **av){
	g_signal = false;
	signal(SIGINT, Signal_handler);

	if (ac != 3) {
		std::cout << "wrong arguments!\n" << "<program> <port>  <password>" << std::endl; return 1;
	}
	try {

	}
	catch (std::exception& err) {
		std::cerr << err.what() << std::endl;
		return -1;
	}

	//listening here is the socket file descriptor
	//	create a socket
	int listening = socket(AF_INET, SOCK_STREAM, 0);//creates an endpoint for communication
	if (listening == -1)
	{
		std::cout << "can't create socket" << std::endl;
		return 1;
	}

	//	bind the socket to a IP / port
	sockaddr_in hint;//socket address structure
	hint.sin_family = AF_INET;//address family of socket
	hint.sin_port = htons(54000);//correct byte order
	inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);//ignore this it's to convert ipv* addresses from text to binary

	//cast socaddr* because it won't see the right one this is how you do it 
	if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1) //bind address
	{
		std::cerr << "Can't bind address" << std::endl;
		return -1;
	}
	//	Mark the socket for listening in
	if (listen(listening, SOMAXCONN) == -1)
	{
		std::cerr << "Can't listen" << std::endl;
		return -3;
	}
	///+++ before accepting a call make a pollfd struct for multiplexing  
	///+++ then use poll() which will wait for one of the file descriptors to become ready to perform I/O
	///+++ this step is for irc since irc listens to multiple fds through the same port 
	///+++ the rest of this main is when you create a new client, you found a polld trying to connect(through an event)
	///+++ so you accept the call and create a new client 
	//pollfd fd_server = {listening, POLLIN, 0};
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
	//			if (_pollfds[i].fd == listening)
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
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	char host[NI_MAXHOST];//max value of maxlen// host name
	char service[NI_MAXSERV];//max value for servlen// port the client is on

	int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);//accept connection to socket 
	if (clientSocket == -1){
		std::cerr << "Error with accepting new client" << std::endl;
		return -4;
	}

	//	Close the listening socket
	close(listening);//not in irc// close listening port since we don't need it  

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

/*
	Sure, here's a list of the typical responses an IRC server should send to the client for each of these commands. This ensures the client receives the appropriate feedback and information as per the IRC protocol.

### Responses to IRC Commands

1. **PASS**
   - **Command**: `/pass`
   - **Description**: Sets a password for connecting to the server.
   - **Response**: No direct response is typically required if the password is correct. If incorrect, a server error message should be sent.
     ```irc
     :server 464 * :Password incorrect
     ```

2. **NICK**
   - **Command**: `/nick`
   - **Description**: Sets or changes the client's nickname.
   - **Response**: If the nickname is valid and available:
     ```irc
     :oldnick!user@host NICK :newnick
     ```
     If the nickname is already in use:
     ```irc
     :server 433 * newnick :Nickname is already in use
     ```

3. **OPER**
   - **Command**: `/oper`
   - **Description**: Authenticates the user as an IRC operator.
   - **Response**: If the credentials are correct:
     ```irc
     :server 381 user :You are now an IRC operator
     ```
     If the credentials are incorrect:
     ```irc
     :server 491 user :No O-lines for your host
     ```

4. **USER**
   - **Command**: Automatically sent by the client during the connection process.
   - **Description**: Specifies the username, hostname, and real name of the user.
   - **Response**: Typically, there is no immediate response to `USER`, but after successful `NICK` and `USER`, the server should send a welcome message:
     ```irc
     :server 001 nick :Welcome to the Internet Relay Network nick!user@host
     :server 002 nick :Your host is server, running version 1.0
     :server 003 nick :This server was created date
     :server 004 nick server 1.0 iwso ov
     ```

5. **PRIVMSG**
   - **Command**: `/msg`
   - **Description**: Sends a private message to a user or channel.
   - **Response**: No direct server response is required for `PRIVMSG`. It is delivered to the target user or channel:
     ```irc
     :sender!user@host PRIVMSG target :message
     ```

6. **JOIN**
   - **Command**: `/join`
   - **Description**: Joins a specified channel.
   - **Response**: If the join is successful:
     ```irc
     :nick!user@host JOIN :#channel
     :server 332 nick #channel :Current channel topic
     :server 333 nick #channel user time
     :server 353 nick = #channel :nick1 nick2 nick3
     :server 366 nick #channel :End of /NAMES list
     ```
     If the channel is invite-only or other restrictions apply:
     ```irc
     :server 473 nick #channel :Cannot join channel (+i)
     ```

7. **KILL**
   - **Command**: `/kill`
   - **Description**: Disconnects a user from the server.
   - **Response**: If the kill is successful:
     ```irc
     :oper!user@host KILL target :reason
     ```
     If not authorized:
     ```irc
     :server 481 nick :Permission Denied- You're not an IRC operator
     ```

8. **PING**
   - **Command**: Sent automatically by the server/client to check the connection.
   - **Description**: Tests the presence of an active client or server.
   - **Response**: The server should respond with a `PONG` message:
     ```irc
     :server PONG server :token
     ```

9. **PART**
   - **Command**: `/part`
   - **Description**: Leaves a specified channel.
   - **Response**: If the part is successful:
     ```irc
     :nick!user@host PART #channel :message
     ```

10. **LIST**
    - **Command**: `/list`
    - **Description**: Lists all channels or those matching a pattern.
    - **Response**: The server should list all channels and their topics:
      ```irc
      :server 321 nick Channel :Users  Name
      :server 322 nick #channel1 5 :Channel 1 topic
      :server 322 nick #channel2 10 :Channel 2 topic
      :server 323 nick :End of /LIST
      ```

11. **NAMES**
    - **Command**: `/names`
    - **Description**: Lists all users in a channel.
    - **Response**: The server should send the list of names in the channel:
      ```irc
      :server 353 nick = #channel :nick1 nick2 nick3
      :server 366 nick #channel :End of /NAMES list
      ```

12. **TOPIC**
    - **Command**: `/topic`
    - **Description**: Sets or gets the topic of a channel.
    - **Response**: If getting the topic:
      ```irc
      :server 332 nick #channel :Current channel topic
      :server 333 nick #channel user time
      ```
      If setting the topic:
      ```irc
      :nick!user@host TOPIC #channel :New topic
      ```

13. **KICK**
    - **Command**: `/kick`
    - **Description**: Removes a user from a channel.
    - **Response**: If the kick is successful:
      ```irc
      :nick!user@host KICK #channel target :reason
      ```

14. **MODE**
    - **Command**: `/mode`
    - **Description**: Changes or queries the mode of a channel or user.
    - **Response**: If querying:
      ```irc
      :server 324 nick #channel +nt
      ```
      If setting:
      ```irc
      :nick!user@host MODE #channel +o nick
      ```

15. **NOTICE**
    - **Command**: `/notice`
    - **Description**: Sends a notice to a user or channel.
    - **Response**: No direct server response is required. It is delivered to the target user or channel:
      ```irc
      :sender!user@host NOTICE target :message
      ```

### Summary

The responses provided above align with the standard IRC protocol. Proper implementation of these responses ensures that your server communicates correctly with IRC clients like Irssi, providing the expected feedback and maintaining compatibility with the IRC network.

*/
