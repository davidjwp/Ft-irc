/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djacobs <djacobs@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 12:37:14 by prossi            #+#    #+#             */
/*   Updated: 2024/05/24 17:57:51 by djacobs          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../irc.hpp"

int Server::cmdPing(std::vector<String> args, Client &cl)
{
    if (args.size() < 2)
	{
		cl.reply(ERROR_NEED_MORE_PARAMETERS(cl, "Ping"));
		return (-1);
	}
    cl.reply("Pong " + args[1]);
    return (0);
}