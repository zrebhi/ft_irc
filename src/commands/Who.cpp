/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 21:53:20 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/09 23:06:24 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::who(Server ircServ, Client user) {
	if (this->_commandArray[1][0] == '#' && checkChannelExists(ircServ, user))
		whoChannel(ircServ);
}

void Command::whoChannel(Server ircServ) {
	std::map<std::string, Channel> channels = ircServ.getChannelList();
	std::string	channelName = this->_commandArray[1].substr(1);

	std::map<int, Client>	channelUsers = channels[channelName].getUsers();
	std::map<int, Client>::iterator	it = channelUsers.begin();

	for (; it != channelUsers.end(); it++) {
		std::string	reply = ":IRC 353 " + it->second.getNickname() + " = " + this->_commandArray[1] + " :" \
			+ channels[channelName].getUserList();
		ft_send(it->first, reply);
	}
}