/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:49:00 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 12:49:23 by moboigui         ###   ########.fr       */
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
		Channel	newChannel(channelName);
		channels.insert(std::make_pair(channelName, newChannel));
		channels[channelName].addOperator(this->_client);
		if (_commandArray.size() == 3)
		{
			channels[channelName].setPassword(password, _client.getNickname(), true);
		}
	}
	if (channels[channelName].checkPassword(password))
	{
		channels[channelName].addUser(this->_client);
		std::string joinMessage = ":" + this->_client.getNickname() + " JOIN :#" + channelName;
		std::map<std::string, Client>::iterator it = channels[channelName].getUsers().begin();
		for (; it != channels[channelName].getUsers().end(); it++)
			ft_send(it->second, joinMessage);
		this->names(channels[channelName]);
	}
}

void Command::list(std::map <std::string, Channel> &channels) {
	std::map<std::string, Channel>::iterator it = channels.begin();
	for (; it != channels.end(); it++)
		std::cout << it->first << std::endl;
}
