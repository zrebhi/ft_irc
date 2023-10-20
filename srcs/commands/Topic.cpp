/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/19 17:29:20 by ubuntu            #+#    #+#             */
/*   Updated: 2023/10/19 18:20:33 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::topic() {
    std::string channelName = this->_commandArray[1];

    if (!channelExists(channelName)) {
        ft_send(this->_client, ":IRC 403 " + this->_client.getNickname() + " " + channelName + " :No such channel");
        return;
    }

    Channel &channel = this->_ircServ.getChannel(channelName.substr(1));

    if (this->_commandArray.size() < 3) {
        ft_send(this->_client, ":IRC 461 " + this->_client.getNickname() + " TOPIC :Not enough parameters");
        return;
    }

    if (!channel.isUserInChannel(this->_client.getNickname())) {
        ft_send(this->_client, ":IRC 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel");
        return;
    }

    std::string topicText;
    for (size_t i = 2; i < this->_commandArray.size(); ++i) {
        topicText += this->_commandArray[i];
        if (i < this->_commandArray.size() - 1)
            topicText += " ";
    }

    if (!channel.isOperator(this->_client.getNickname())) {
        ft_send(this->_client, ":IRC 482 " + this->_client.getNickname() + " " + channelName + " :You're not a channel operator");
        return;
    }

    channel.setTopic(channel.getName(), this->_client.getNickname(), topicText);

    ft_send(this->_client, ":IRC 332 " + this->_client.getNickname() + " " + channelName + " :" + topicText);
}
