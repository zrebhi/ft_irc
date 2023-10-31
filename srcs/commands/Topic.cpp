/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 17:29:20 by ubuntu            #+#    #+#             */
/*   Updated: 2023/10/23 19:19:05 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::topic() {
	if (_commandArray.size() < 2)
		return ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "TOPIC"));

	std::string channelName = this->_commandArray[1];
	channelName = formatChannelName(channelName);

	if (!channelExists(channelName))
		return;

	Channel &channel = this->_ircServ.getChannel(channelName);

    if (!channel.isUserInChannel(this->_client.getNickname()))
        return (ft_send(this->_client, ERR_NOTONCHANNEL(channelName)));

    if (this->_commandArray.size() == 2 && channel.getTopic().empty())
		return (ft_send(this->_client, RPL_NOTOPIC(channelName)));

    if (this->_commandArray.size() == 2)
		return ft_send(this->_client,RPL_TOPIC(this->_client, channelName, channel.getTopic()));

    if (channel.isTopicLocked() && !channel.isOperator(this->_client.getNickname())) {
        ft_send(this->_client, ERR_CHANOPRIVSNEEDED(channelName, this->_client));
		ft_send(this->_client, RPL_TOPIC(this->_client, channelName, channel.getTopic()));
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
