/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 23:24:53 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 12:49:01 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <map>

void Command::user() {
	this->_client.setUsername(this->_commandArray[1]);
	std::string reply = ":IRC 001 " + this->_client.getNickname() + " :Welcome to the Internet Relay Network " +
			this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + this->_client.getHostname();
	ft_send(this->_client, reply);
}

void Command::nick(std::map<int, Client> &clientList) {
	// (void)clientList;
	std::map<int, Client>::iterator it = clientList.begin();
	while (it != clientList.end())
	{
		if (it->second.getNickname() == _commandArray[1])
		{
			std::string reply = ":IRC 433 " + _commandArray[1] + " :NICK in use.\n";
			ft_send(this->_client, reply);
			return; // send error
		}
		it++;
	}
	std::string oldNick = _client.getNickname();
	if (oldNick.empty())
		oldNick = '*';
	this->_client.setNickname(this->_commandArray[1]);
	std::string reply = ":" + oldNick + "!" + _client.getUsername() + "@" + _client.getHostname() + " NICK :" + _client.getNickname();
	ft_send(this->_client, reply);
}
