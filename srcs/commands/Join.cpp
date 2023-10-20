/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:49:00 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/20 22:32:31 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::join() {
	if (_commandArray.size() < 2 || _commandArray[1].empty())
		ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, _commandArray[0]));

	std::map<std::string, std::string> joinMap = joinParser();
	std::map<std::string, std::string>::iterator it = joinMap.begin();
	for (; it != joinMap.end(); it++) {
		joinChannel(it->first, it->second);
	}
}

std::map<std::string, std::string>	Command::joinParser() {
	std::map<std::string, std::string>	joinMap;
	std::vector<std::string>	channelNames = ft_split(_commandArray[1], ',');
	std::vector<std::string>	channelPasswords;
	if (_commandArray.size() == 3)
		channelPasswords = ft_split(_commandArray[2], ',');
	for (size_t i = 0; i < channelNames.size(); i++) {
		if (i < channelPasswords.size())
			joinMap[channelNames[i]] = channelPasswords[i];
		else
			joinMap[channelNames[i]] = "";
	}
	return joinMap;
}

void	Command::joinChannel(std::string channelName, std::string channelPassword) {
	if (!validChannelName(channelName))
		return ft_send(this->_client, ERR_INVALIDCHANNEL(this->_client, channelName));

	channelName = formatChannelName(channelName);
	if (!channelExists(channelName))
		createChannel(channelName, channelPassword);
	else {
		Channel& channel = this->_ircServ.getChannel(channelName);
		if (permissionToJoinChannel(channel, channelName, channelPassword)) {
			channel.setInvitedList(_client.getNickname(), REMOVE);
			channel.addUser(this->_client);
			ft_send(this->_client, RPL_JOIN(this->_client, channelName));
		}
	}
}

bool Command::validChannelName(std::string channelName) {
	if (channelName[0] != '#' || channelName.size() < 2)
		return false;
	std::string allowedSpecialChars = "-[]`^{}\'\"";
	for (size_t i = 1; i < channelName.size(); i++) {
		if (!std::isalnum(channelName[i]) && allowedSpecialChars.find(channelName[i]) == allowedSpecialChars.npos)
			return false;
	}
	return true;
}

std::string Command::formatChannelName(std::string channelName) {
	channelName = channelName.substr(1);
	for (size_t i = 0; i < channelName.size(); i++) {
		channelName[i] = std::tolower(channelName[i]);
	}
	return channelName;
}

void Command::createChannel(std::string channelName, std::string channelPassword) {
	this->_ircServ.addChannelToServer(Channel(channelName));
	Channel&	newChannel = this->_ircServ.getChannel(channelName);

	newChannel.addUser(this->_client);
	newChannel.addOperator(this->_client);
	newChannel.setChannelPassword(channelPassword, _client.getNickname(), true);
	ft_send(this->_client, RPL_JOIN(this->_client, channelName));
}

bool Command::permissionToJoinChannel(Channel& channel, std::string channelName, std::string channelPassword) {
	if (channel.isUserInChannel(this->_client.getNickname()))
		return false;
	if (!channel.checkChannelPassword(channelPassword)) {
		ft_send(this->_client, ERR_INCORRECTPASSWORD(this->_client, channelName));
		return false;
	}
	else if (channel.isFull()) {
		ft_send(this->_client, ERR_CHANNELISFULL(this->_client, channelName));
		return false;
	}
	else if (channel.isInviteOnly() && !channel.isInvited(_client.getNickname())) {
		ft_send(this->_client, ERR_INVITEONLYCHAN(channelName));
		return false;
	}
	return true;
}
