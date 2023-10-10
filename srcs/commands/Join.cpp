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
	std::string	channelName = this->_commandArray[1].substr(1);

	std::map<std::string, Channel>::iterator it = channels.find(channelName);

	if (it == channels.end()) {
		Channel	newChannel(channelName);
		channels.insert(std::make_pair(channelName, newChannel));
		channels[channelName].addOperator(this->_client);
	}
	channels[channelName].addUser(this->_client);

	std::string joinMessage = ":" + this->_client.getNickname() + " JOIN :#" + channelName;
	ft_send(this->_client, joinMessage);
}

void Command::list(std::map <std::string, Channel> &channels) {
	std::map<std::string, Channel>::iterator it = channels.begin();
	for (; it != channels.end(); it++)
		std::cout << it->first << std::endl;
}