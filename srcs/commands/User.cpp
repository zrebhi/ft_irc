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
#include <cctype>
#include <iostream>
#include <map>

void Command::user() {
	std::string &username = _commandArray[1];
	size_t firstSpace = username.find(' ');

	std::cout << "username: " << username << std::endl;
	if (username.empty())
		username = "guest";
	else if (firstSpace != username.npos)
		username = username.substr(0, firstSpace);
	std::cout << "username: " << username << std::endl;
	this->_client.setUsername(username);
}

void Command::nick() {
	std::string newNickname = this->_commandArray[1];
	std::string oldNickname = this->_client.getNickname();

	if (oldNickname.empty())
		oldNickname = '*';
	if (nicknameAvailable(newNickname) && nicknameIsValid(newNickname)) {
		this->_client.setNickname(newNickname);
		this->_client.setRegistered(NICK_REGISTRATION);
	}
}

bool Command::nicknameAvailable(std::string nickname) {
	std::map<int, Client> clientList = this->_ircServ.getClientList();
	std::map<int, Client>::iterator it = clientList.begin();

	for (; it != clientList.end(); ++it) {
		if (it->second.getNickname() == nickname && it->first != this->_client.getSocket()) {
			ft_send(this->_client, ERR_NICKNAMEINUSE(nickname));
			return false;
		}
	}
	return true;
}

bool Command::nicknameIsValid(std::string nickname) {
	if (nickname.length() < 3)
		return false;
	std::string nonAlnumValidChars = "-_^[]{}\\`|";
	if (nickname.at(0) == '-')
		return false;
	for (size_t i = 0; i < nickname.length(); i++)
	{
		char letter = nickname.at(i);
		if (!isalnum(letter) && nonAlnumValidChars.find(letter) == std::string::npos)
		{
			ft_send(this->_client, ERR_ERRONEUSNICKNAME(nickname));
			return false;
		}
	}
	return true;
}
