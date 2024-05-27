#include <poll.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>

#define Bblue "\033[1;34m"
#define Bred "\033[1;31m"
#define Bgreen "\033[1;32m"
#define Bcyan "\033[1;36m"
#define Byellow "\033[1;33m"
#define rescol "\033[0m"
#define colstring(col, msg)(col + msg + rescol)

typedef std::string String;

enum stat{
	ONE = 1,
	TWO = 2,
	THREE = 4,
};

/*

	//SET POLL
	pollfd fd_server = {_sock, POLLIN, 0};
	_pollfds.push_back(fd_server);
	
	std::cout << "Server IRC Launched!" << std::endl;
	while (!g_signal)
	{
		if (poll(_pollfds.begin().base(), _pollfds.size(), -1) < 0)
			break ;
		for (unsigned int i = 0; i < _pollfds.size(); i++)

*/

std::vector<pollfd> ok;


int	main(int ac, char** av){
	String msg("  my	message   ");
	std::vector<String> messages;


	while (msg.find('\t') != std::string::npos)
		msg[msg.find('\t')] = 32;

	std::stringstream str(msg);
	String tmp(msg);
	
	while (getline(str ,tmp, ' '))
		if (tmp.size()) messages.push_back(tmp);
	
	for (std::vector<String>::iterator it = messages.begin(); it != messages.end(); it++)
		std::cout << "$" << *it << "$" << std::endl;

	String	test("");
	int	state = ONE + TWO + THREE;

	if (state & ONE && state & TWO && state & THREE)
		std::cout << colstring(Bgreen, String("GOOD")) << std::endl;

	std::cout << colstring(Bblue, msg) << " and the rest" << std::endl;
	std::cout << colstring(Bgreen, msg) << " and the rest" << std::endl;
	std::cout << colstring(Bred, msg) << " and the rest" << std::endl;
	std::cout << colstring(Bcyan, msg) << " and the rest" << std::endl;
	std::cout << colstring(Byellow, msg) << " and the rest" << std::endl;

	return 0; 
}