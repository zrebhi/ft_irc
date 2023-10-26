/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 21:53:20 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/19 21:32:47 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <ostream>

void	Command::who() {
	if (_commandArray.size() < 2 || _commandArray[1].empty())
		return ft_send(_client, ERR_NEEDMOREPARAMS(_client, _commandArray[0]));

	if (this->_commandArray[1][0] == '#' && channelExists(this->_commandArray[1]))
		whoChannel(this->_commandArray[1]);
}

void Command::whoChannel(std::string channelName) {
	Channel &channel = this->_ircServ.getChannel(channelName);

	std::map<std::string, Client>	channelUsers = channel.getUsers();
	std::map<std::string, Client>::iterator	it = channelUsers.begin();

	for (; it != channelUsers.end(); it++) {
		ft_send(it->second, RPL_NAMERPLY(it->second, channel));
	}
}
