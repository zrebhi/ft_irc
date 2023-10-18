/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 21:53:20 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/18 23:00:14 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::who() {
	if (this->_commandArray.size() < 2)
		return;
	if (this->_commandArray[1][0] == '#' && channelExists(this->_commandArray[1]))
		whoChannel(this->_commandArray[1]);
}

void Command::whoChannel(std::string channelName) {
	Channel &channel = this->_ircServ.getChannel(channelName);

	std::map<std::string, Client>	channelUsers = channel.getUsers();
	std::map<std::string, Client>::iterator	it = channelUsers.begin();

	for (; it != channelUsers.end(); it++) {
		ft_send(it->second.getSocket(), RPL_NAMERPLY(it->second, channel));
	}
}