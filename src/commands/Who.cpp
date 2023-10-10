/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 21:53:20 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/10 21:25:53 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::who() {
	if (this->_commandArray[1][0] == '#' && ChannelExists())
		whoChannel();
}

void Command::whoChannel() {
	std::map<std::string, Channel> channels = _ircServ.getChannelList();
	std::string	channelName = this->_commandArray[1].substr(1);

	std::map<std::string, Client>	channelUsers = channels[channelName].getUsers();
	std::map<std::string, Client>::iterator	it = channelUsers.begin();

	for (; it != channelUsers.end(); it++) {
		std::string	reply = ":IRC 353 " + it->second.getNickname() + " = " + this->_commandArray[1] + " :" \
			+ channels[channelName].UserListString();
		ft_send(it->second.getSocket(), reply);
	}
}