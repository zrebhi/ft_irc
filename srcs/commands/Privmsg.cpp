/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 21:40:45 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/11 21:34:38 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::privmsg() {
	if (this->_commandArray[1][0] == '#' && \
	channelExists(this->_commandArray[1]) && \
	IsChannelMember(this->_client.getNickname(), this->_commandArray[1]))
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
	channel.userMessageToChannel(this->_client, message);
}
