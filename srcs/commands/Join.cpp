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
		if (!channel.checkPassword(password))
			ft_send(this->_client, ERR_INCORRECTPASSWORD(this->_client, channelName));
		else {
			channel.addUser(this->_client);
			ft_send(this->_client, RPL_JOIN(this->_client, channelName));
		}
	}
}

void Command::createChannel(std::string channelName, std::string password) {
	this->_ircServ.addChannelToServer(Channel(channelName));
	Channel&	newChannel = this->_ircServ.getChannel(channelName);

	newChannel.setPassword(password, _client.getNickname());
	newChannel.addUser(this->_client);
	newChannel.addOperator(this->_client);
	ft_send(this->_client, RPL_JOIN(this->_client, channelName));
}
