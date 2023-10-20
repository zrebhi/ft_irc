/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 17:29:20 by ubuntu            #+#    #+#             */
/*   Updated: 2023/10/21 00:07:46 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::topic() {
	if (_commandArray.size() < 3 || _commandArray[1].empty() || _commandArray[2].empty())
		return ft_send(_client, ERR_NEEDMOREPARAMS(_client, _commandArray[0]));

	std::string channelName = this->_commandArray[1];

	if (!validChannelName(channelName))
		return ft_send(this->_client, ERR_INVALIDCHANNEL(this->_client, channelName));

	channelName = formatChannelName(channelName);

	if (!channelExists(channelName))
		return ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client,channelName));

	Channel &channel = this->_ircServ.getChannel(channelName);

	if (!channel.isUserInChannel(this->_client.getNickname()))
		return ft_send(this->_client, ERR_NOTONCHANNEL(channelName));


	std::string topicText;
	for (size_t i = 2; i < this->_commandArray.size(); ++i) {
		topicText += this->_commandArray[i];
		if (i < this->_commandArray.size() - 1)
			topicText += " ";
	}

	if (!channel.isOperator(this->_client.getNickname()))
		return ft_send(this->_client, ERR_CHANOPRIVSNEEDED(channelName, _client));

	channel.setTopic(this->_client.getNickname(), topicText);

	ft_send(this->_client, RPL_TOPIC(this->_client, channelName, topicText));
}
