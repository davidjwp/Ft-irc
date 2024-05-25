#include <poll.h>
#include <iostream>
#include <vector>


#define Bblue "\033[1;34m"
#define Bred "\033[1;31m"
#define Bgreen "\033[1;32m"
#define Bcyan "\033[1;36m"
#define Byellow "\033[1;33m"
#define rescol "\033[0m"
#define colstring(col, msg)(col + msg + rescol)

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
	std::string msg("my message");

	std::string	test("");
	int	state = ONE + TWO + THREE;

	while (test.find("\n") == std::string::npos)
	{
		std::cout << "ok" << std::endl;
	}

	if (state & ONE && state & TWO && state & THREE)
		std::cout << colstring(Bgreen, std::string("GOOD")) << std::endl;

	std::cout << colstring(Bblue, msg) << " and the rest" << std::endl;
	std::cout << colstring(Bgreen, msg) << " and the rest" << std::endl;
	std::cout << colstring(Bred, msg) << " and the rest" << std::endl;
	std::cout << colstring(Bcyan, msg) << " and the rest" << std::endl;
	std::cout << colstring(Byellow, msg) << " and the rest" << std::endl;

	return 0; 
}