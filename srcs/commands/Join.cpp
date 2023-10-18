/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:49:00 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 23:36:51 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::join() {
	std::string channelName = this->_commandArray[1];
	std::string password;

	if (channelName.at(0) == '#')
		channelName = channelName.substr(1);
	if (_commandArray.size() == 3)
		password = _commandArray[2];

	if (!channelExists(channelName))
		createChannel(channelName, password);
	else {
		Channel& channel = this->_ircServ.getChannel(channelName);
		if (!channel.checkChannelPassword(password))
			ft_send(this->_client, ERR_INCORRECTPASSWORD(this->_client, channelName));
		else if (channel.isFull())
			ft_send(this->_client, ERR_CHANNELISFULL(this->_client, channelName));
		else if (channel.isInviteOnly() && !channel.isInvited(_client.getNickname()))
			ft_send(this->_client, ERR_INVITEONLYCHAN(channelName));
		else {
			channel.setInvitedList(_client.getNickname(), REMOVE);
			channel.addUser(this->_client);
			ft_send(this->_client, RPL_JOIN(this->_client, channelName));
		}
	}
}

void Command::createChannel(std::string channelName, std::string password) {
	this->_ircServ.addChannelToServer(Channel(channelName));
	Channel&	newChannel = this->_ircServ.getChannel(channelName);

	newChannel.setChannelPassword(password, _client.getNickname(), true);
	newChannel.addUser(this->_client);
	newChannel.addOperator(this->_client);
	ft_send(this->_client, RPL_JOIN(this->_client, channelName));
}
