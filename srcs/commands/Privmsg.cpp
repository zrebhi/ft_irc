/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 21:40:45 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/23 21:36:11 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::privmsg() {
	if (_commandArray.size() < 3 || _commandArray[1].empty() || _commandArray[2].empty())
		return ft_send(_client, ERR_NEEDMOREPARAMS(_client, _commandArray[0]));

	if (_commandArray[1].at(0) == '#')
		channelMessage();
	else
		privateMessage();
}

void Command::channelMessage()
{
	std::string channelName = "#" + formatChannelName(this->_commandArray[1]);
	if (!validChannelName(channelName))
		return ft_send(this->_client, ERR_INVALIDCHANNEL(this->_client, channelName));
	if (!IsChannelMember(this->_client.getNickname(), channelName))
		ft_send(_client, ERR_NOTONCHANNEL(channelName));
	else if (!channelExists(channelName))
		ft_send(_client, ERR_NOSUCHCHANNEL(_client, channelName));
	else
	{
		std::string message;
		for (size_t i = 2; i < this->_commandArray.size(); i++) {
			message.append(" ");
			message.append(this->_commandArray[i]);
		}
		if (message.size() >= 300)
			return ft_send(_client, ERR_TOOMANYMATCHES(_client));
		message = RPL_PRIVMSG(_client, channelName, message);
		Channel channel = this->_ircServ.getChannel(channelName);
		channel.userMessageToChannel(this->_client, message);
	}
}


void Command::privateMessage()
{
	std::map<int, Client>::iterator it = findClientOnServer(_commandArray[1]);
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
		if (message.size() >= 300)
			return ft_send(_client, ERR_TOOMANYMATCHES(_client));
		ft_send(it->second, RPL_PRIVMSG(_client, it->second.getNickname(), message));
	}
}

