/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 21:53:20 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/11 01:26:36 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::who() {
	if (this->_commandArray[1][0] != '#' || !channelExists(this->_commandArray[1]))
		return;
	Channel channel = this->_ircServ.getChannel(this->_commandArray[1]);

	std::map<std::string, Client>	channelUsers = channel.getUsers();
	std::map<std::string, Client>::iterator	it = channelUsers.begin();

	for (; it != channelUsers.end(); it++) {
		ft_send(it->second.getSocket(), RPL_WHOREPLY(it->second.getNickname(), _commandArray[1], channel.userListString()));
	}
}
