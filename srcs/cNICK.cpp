#include "irc.hpp"

void Server::cNICK(std::vector<std::string> messages, int fd){
	std::vector<Client>::iterator cl = getClient(fd);
	std::vector<std::string>::iterator msg;

	if (messages.size() > 1)
		msg = messages.begin() + 1;
	else return ;

	for (struct {const char* forbidden; int i;} loop = {" ,*?!@.", 0}; loop.forbidden[loop.i] != 0; loop.i++) {
		if (msg->find(loop.forbidden[loop.i], 0) != std::string::npos){
			std::cout << colstring(Bred, std::string("Warning\t")) << 
			"characters ' ' ',' '*' '?' '!' '@' '.' are not allowed in nickname." << std::endl;
			return ;
		}
	}
	if ((*msg)[0] == '$' || (*msg)[0] == ':' || (*msg)[0] == '$'){
		std::cout << colstring(Bred, std::string("Warning\t")) <<
		"characters '$' ':' '#' '&' are not allowed for first character in nickname" << std::endl;
		return ;
	}

	cl->Set_nick(*msg);
	cl->isRegistered();
}