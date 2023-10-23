/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:49:00 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/21 01:20:49 by zrebhi           ###   ########.fr       */
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
			channel.serverMessageToChannel(RPL_JOIN(this->_client.getNickname(), channelName));
			if (!channel.getTopic().empty())
				ft_send(_client, RPL_TOPIC(_client, channelName, channel.getTopic()));
		}
		else
			return;
	}
	ft_send(_client, currentModesStr());
}

void Command::createChannel(std::string channelName, std::string channelPassword) {
	this->_ircServ.addChannelToServer(Channel(channelName));
	Channel&	newChannel = this->_ircServ.getChannel(channelName);

	newChannel.addUser(this->_client);
	newChannel.addOperator(this->_client);
	if (!channelPassword.empty())
		newChannel.setChannelPassword(channelPassword, _client.getNickname(), true);
	newChannel.serverMessageToChannel(RPL_JOIN(this->_client.getNickname(), channelName));
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
