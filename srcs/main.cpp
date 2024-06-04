#include "irc.hpp"

void	Signal_handler(const int signal) {
	(void)signal;
	stop_server = true;
	std::cout << "\b\b";//clean tty exit
}

int	Check_port(char* arg) {
	char *endptr;
	long int port = strtol(arg, &endptr, 10);

	if (*endptr != '\0') throw Error("Error: Check_port only digits in port allowed.");
	if (port > 65535 || port < 0) throw Error("Error: Check_port wrong port range.");
	return port;
}

int	main(int ac, char **av) {
	signal(SIGINT, Signal_handler);

	if (ac != 3) { std::cout << colstring(BBred, std::string("wrong arguments!")) << "\n<program> <port>  <password>" << std::endl; return 1;}
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