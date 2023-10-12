/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:49:00 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/10 20:04:19 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::join(std::map <std::string, Channel> &channels) {
	std::string	channelName = this->_commandArray[1];
	if (channelName.at(0) == '#')
		channelName = channelName.substr(1);
	std::map<std::string, Channel>::iterator it = channels.find(channelName);
	std::string password = "NO";
	if (_commandArray.size() == 3)
		password = _commandArray[2];
	if (it == channels.end()) {
		std::cout << "newChannel" << std::endl;
		Channel	newChannel(channelName);
		channels.insert(std::make_pair(channelName, newChannel));
		channels[channelName].addOperator(this->_client);
		if (_commandArray.size() == 3)
		{
			channels[channelName].setPassword(password, _client.getNickname());
			std::cout << "newPassword" << password << std::endl;
		}
	}
	if (channels[channelName].checkPassword(password))
	{
		std::cout << "ok checkPassword = " << password << std::endl;
		channels[channelName].addUser(this->_client);
		std::string joinMessage = ":" + this->_client.getNickname() + " JOIN :#" + channelName;
		ft_send(this->_client, joinMessage);
	}
}

void Command::list(std::map <std::string, Channel> &channels) {
	std::map<std::string, Channel>::iterator it = channels.begin();
	for (; it != channels.end(); it++)
		std::cout << it->first << std::endl;
}
