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
	if (_commandArray.size() < 2)
		return ft_send(_client, ":IRC 461 " + this->_client.getNickname() + " TOPIC :Not enough parameters");

	std::string channelName = this->_commandArray[1];
	channelName = formatChannelName(channelName);

	if (!channelExists(channelName))
		return ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));

	Channel &channel = this->_ircServ.getChannel(channelName);


    if (!channel.isUserInChannel(this->_client.getNickname())) {
        ft_send(this->_client, ":IRC 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel");
        return;
    }

    if (this->_commandArray.size() == 2 && channel.getTopic().empty()) {
		ft_send(this->_client, ":IRC 331 " + _client.getNickname() + " " + _commandArray[1] + " :No topic is set");
        return;
    }

    if (this->_commandArray.size() == 2) {
		ft_send(this->_client, ":IRC 332 " + this->_client.getNickname() + " " + channelName + " " + channel.getTopic());
        return;
    }

    if (channel.isTopicLocked() && !channel.isOperator(this->_client.getNickname())) {
        ft_send(this->_client, ":IRC 482 " + this->_client.getNickname() + " #" + channelName + " :You're not a channel operator");
		ft_send(_client, ":IRC 332 " + _client.getNickname() + " #" + channelName + " " + channel.getTopic());
        return;
    }

    std::string topicText;
    for (size_t i = 2; i < this->_commandArray.size(); ++i) {
        topicText += this->_commandArray[i];
        if (i < this->_commandArray.size() - 1)
            topicText += " ";
    }
	if (topicText == ":")
		topicText = "";

    channel.setTopic(this->_client.getNickname(), topicText);
}
