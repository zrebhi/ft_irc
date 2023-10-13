/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 23:24:53 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/14 00:44:05 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <map>

void Command::user() {
	this->_client.setUsername(this->_commandArray[1]);
}

void Command::nick() {
	std::string newNickname = this->_commandArray[1];
	std::string oldNickname = this->_client.getNickname();

	if (oldNickname.empty())
		this->_client.setNickname(newNickname);
	if (nicknameIsValid(newNickname) && nicknameAvailable(newNickname)) {
		this->_client.setNickname(newNickname);
		ft_send(this->_client, ":" + oldNickname + " NICK :" + newNickname);
	}
}

bool Command::nicknameAvailable(std::string nickname) {
	std::map<int, Client> clientList = this->_ircServ.getClientList();
	std::map<int, Client>::iterator it = clientList.begin();

	for (; it != clientList.end(); ++it) {
		if (it->second.getNickname() == nickname && it->first != this->_client.getSocket()) {
			ft_send(this->_client, ERR_NICKNAMEINUSE(this->_client));
			return false;
		}
	}
	return true;
}

bool Command::nicknameIsValid(std::string nickname) {
	if (nickname.find(' ', 1) != nickname.npos || nickname.find('#') != nickname.npos || \
	nickname.size() > 9 || nickname.size() < 2) {
		ft_send(this->_client, ERR_ERRONEUSNICKNAME(nickname));
		return false;
	}
	else
		return true;
}