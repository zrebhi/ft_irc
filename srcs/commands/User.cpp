/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 23:24:53 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/18 23:26:36 by zrebhi           ###   ########.fr       */
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
		if (!this->_client.isRegistered())
			this->_client.setRegistered(NICK_REGISTRATION);
		changeNicknameInChannels(oldNickname);
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
// bool isValidPassword(const std::string &password) {
// 	std::string errorString = "Invalid password: Password should be between 2 
// 	and 9 alphanumeric characters.\nPlease enter a new one:\t";

//     if (password.length() < 2 || password.length() > 9)
// 	{
// 		std::cout << errorString << std::endl;
//         return false;
// 	}
// 	for (size_t i = 0; i < password.length(); i++)
// 	{
// 		if (!isalnum(password.at(i)))
// 		{
// 			std::cout << errorString << std::endl;
// 			return false;
// 		}
// 	}
//     return true;
// }

bool Command::nicknameIsValid(std::string nickname) {
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

	// if (nickname.find(' ', 1) != nickname.npos || nickname.find('#') != nickname.npos ||
	// nickname.size() > 9 || nickname.size() < 2) {
	// 	ft_send(this->_client, ERR_ERRONEUSNICKNAME(nickname));
	// 	return false;
	// }
	// else
	// 	return true;
}

void Command::changeNicknameInChannels(std::string oldNickname) {
	std::map <std::string, Channel> &channels = this->_ircServ.getChannelList();
	std::map<std::string, Channel>::iterator it = channels.begin();

	for (; it != channels.end(); ++it) {
		if (it->second.isUserInChannel(oldNickname)) {
			Channel	&channel = this->_ircServ.getChannel(it->first);
			channel.addUser(this->_client);
			channel.removeUser(oldNickname);
			channel.serverMessageToChannel(NICK(oldNickname, this->_client.getNickname()));
		}
	}
}