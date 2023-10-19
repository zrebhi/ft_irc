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

void Command::channelMessage()
{
	if (!IsChannelMember(this->_client.getNickname(), this->_commandArray[1]))
		ft_send(_client, ERR_NOTONCHANNEL(_commandArray[1]));
	else if (!channelExists(_commandArray[1]))
		ft_send(_client, ERR_NOSUCHCHANNEL(_client, _commandArray[1]));
	else
	{
		std::string message;
		for (size_t i = 2; i < this->_commandArray.size(); i++)
		{
			message.append(" ");
			message.append(this->_commandArray[i]);
		}
		message = RPL_PRIVMSG(_client, _commandArray[1], message);
		Channel channel = this->_ircServ.getChannel(this->_commandArray[1]);
		channel.userMessageToChannel(this->_client, message);
	}
}

void Command::privateMessage()
{
	const std::map<int, Client>::const_iterator it = findClientOnServer(_commandArray[1]);
	if (it == _ircServ.getClientList().end())
		ft_send(_client, ERR_NOSUCHNICK(_commandArray[1]));
	else
	{
		std::string message;
		for (size_t i = 2; i < this->_commandArray.size(); i++)
		{
			message.append(" ");
			message.append(this->_commandArray[i]);
		}
		ft_send(it->second, RPL_PRIVMSG(_client, it->second.getNickname(), message));
	}
}


void Command::privmsg() {
	if (_commandArray.size() < 3)
	{
		ft_send(_client, ERR_NEEDMOREPARAMS(_client, _commandArray[0]));
		return;
	}
	if (_commandArray[1].at(0) == '#')
			channelMessage();
	else
			privateMessage();
}

