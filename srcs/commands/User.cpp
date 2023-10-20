/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 23:24:53 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/19 21:14:57 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::user() {
	if (_commandArray.size() < 2 || _commandArray[1].empty())
		return ft_send(_client, ERR_NEEDMOREPARAMS(_client, _commandArray[0]));

	std::string &username = _commandArray[1];
	size_t firstSpace = username.find(' ');

	if (username.empty())
		username = "guest";
	else if (firstSpace != username.npos)
		username = username.substr(0, firstSpace);
	this->_client.setUsername(username);
}

void Command::nick() {
	if (_commandArray.size() < 2 || _commandArray[1].empty())
		return ft_send(_client, ERR_NEEDMOREPARAMS(_client, _commandArray[0]));

	std::string newNickname = this->_commandArray[1];
	std::string oldNickname = this->_client.getNickname();

	if (oldNickname.empty())
		oldNickname = '*';
	if (nicknameAvailable(newNickname) && nicknameIsValid(newNickname)) {
		this->_client.setNickname(newNickname);
		if (!this->_client.isRegistered())
			this->_client.setRegistered(NICK_REGISTRATION);
		ft_send(this->_client, NICK(oldNickname, newNickname));
		changeNicknameInChannels(oldNickname);
	}
}

bool Command::nicknameAvailable(std::string nickname)
{
	if (findClientOnServer(nickname) == _ircServ.getClientList().end())
		return true;
	ft_send(this->_client, ERR_NICKNAMEINUSE(this->_client));
	return false;
}

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
