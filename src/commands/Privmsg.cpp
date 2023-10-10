/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 21:40:45 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/11 01:16:18 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::privmsg() {
	if (this->_commandArray[1][0] == '#' && ChannelExists() && IsChannelMember())
		channelMessage();
}

void Command::channelMessage() {
	std::string message = ":" + this->_client.getNickname() + " ";
	for (size_t i = 0; i < this->_commandArray.size(); i++) {
		message.append(this->_commandArray[i]);
		if (i < this->_commandArray.size() - 1)
			message.append(" ");
	}
	Channel channel = this->_ircServ.getChannel(this->_commandArray[1]);
	channel.sendMessageToChannel(this->_client, message);
}

bool Command::ChannelExists() {
	std::string channelName = this->_commandArray[1].substr(1);

	std::map<std::string, Channel> channelList = this->_ircServ.getChannelList();
	std::map<std::string, Channel>::iterator it = channelList.find(channelName);
	if (it == channelList.end()) {
		std::string reply = ":IRC 403 " + this->_client.getNickname() + " " + channelName + " :No such channel";
		ft_send(this->_client, reply);
		return false;
	}
	else
		return true;
}

bool Command::IsChannelMember() {
	std::string channelName = this->_commandArray[1].substr(1);
	std::map<std::string, Channel> channelList = this->_ircServ.getChannelList();

	std::map<std::string, Client> channelUsers = channelList[channelName].getUsers();
	std::map<std::string, Client>::iterator it = channelUsers.find(this->_client.getNickname());
	if (it == channelUsers.end()) {
		std::string reply = ":IRC 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel";
		ft_send(this->_client, reply);
		return false;
	}
	else
		return true;
}