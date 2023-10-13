/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Who.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 21:53:20 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 11:16:51 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	Command::who() {
	return;
	if (this->_commandArray[1][0] == '#' && channelExists(this->_commandArray[1]))
		whoChannel();
}

void Command::whoChannel() {
	Channel channel = this->_ircServ.getChannel(this->_commandArray[1]);

	std::map<std::string, Client>	channelUsers = channel.getUsers();
	std::map<std::string, Client>::iterator	it = channelUsers.begin();

	for (; it != channelUsers.end(); it++) {
		std::string	reply = ":IRC 352 " + this->_client.getNickname() + " #" + channel.getName() + " " + this->_client.getUsername() + " localhost IRC " + it->second.getNickname() + " H :0 Guest";
		ft_send(this->_client, reply);
	}
	std::string reply = ":IRC 315 " + this->_client.getNickname() + " #" + channel.getName() + " :End of /WHO list.";
	ft_send(this->_client, reply);
}

void Command::names(Channel &channel)
{
	std::string replyList = ":IRC 353 " + this->_client.getNickname() + " = #" + channel.getName() + " :" + channel.userListString();
	ft_send(this->_client, replyList);
	std::string replyEndList = ":IRC 366 " + this->_client.getNickname() + " #" + channel.getName() + " :End of /NAMES list";
	ft_send(this->_client, replyEndList);
}
