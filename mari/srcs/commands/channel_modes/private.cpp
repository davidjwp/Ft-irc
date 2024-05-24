#include "Irc.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Commands.hpp"

void	privateChannelMode(Server *server, mode_struct mode_infos, int const client_fd, std::string mode_str)
{
	std::map<const int, Client>::iterator it_client = server->getClients().find(client_fd);
	(void)it_client;
	std::map<std::string, Channel>::iterator it_channel_target = server->getChannels().find(mode_infos.target);

	size_t pos = it_channel_target->second.getMode().find("p");
	if (mode_str[0] == '+')
	{
		if (pos != std::string::npos) // le mode est deja présent
			return;
		it_channel_target->second.addMode("p");
		broadcastToAllChannelMembers(server, it_channel_target->second, MODE_CHANNELMSG(mode_infos.target, "+p"));
	}
	else // si c'est '-'
	{
		if (pos == std::string::npos) // le mode n'est pas présent
			return;
		it_channel_target->second.removeMode("p");
		broadcastToAllChannelMembers(server, it_channel_target->second, MODE_CHANNELMSG(mode_infos.target, "-p"));
	}
}