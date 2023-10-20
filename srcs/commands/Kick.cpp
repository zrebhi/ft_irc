/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 19:51:53 by bgresse           #+#    #+#             */
/*   Updated: 2023/10/20 23:30:26 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::kick() {
	if (_commandArray.size() < 3 || _commandArray[1].empty() || _commandArray[2].empty())
		return ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, _commandArray[0]));

	if (_commandArray[1] == "IRC")
		_commandArray.erase(_commandArray.begin() + 1);
	std::map<std::string, Channel> &channels = this->_ircServ.getChannelList();
	std::string channelName = this->_commandArray[1];

	if (!validChannelName(channelName))
		 return ft_send(this->_client, ERR_INVALIDCHANNEL(this->_client, channelName));

	channelName = formatChannelName(channelName);
	std::map<std::string, Channel>::iterator channelIt = channels.find(channelName);
	if (channelIt != channels.end()) {
		 Channel &channel = channelIt->second;

		 if (!channel.isUserInChannel(_client.getNickname()))
			 return ft_send(this->_client, ERR_NOTONCHANNEL(channelName));
		 else if (!channel.isOperator(_client.getNickname()))
			  return ft_send(this->_client, ERR_CHANOPRIVSNEEDED(channelName));
		 else {
			  std::string targetNickname = this->_commandArray[2];

			if (!channel.isUserInChannel(targetNickname))
					return ft_send(this->_client, ERR_NOSUCHNICK(targetNickname));

			ft_send(channel.getUsers()[targetNickname], RPL_KICK_USER(_client, channelName));
			std::string kickMessage = RPL_KICK_CHANNEL(_client, channelName, targetNickname, (_commandArray.size() >= 4 ? _commandArray[3] : ""));
			channel.deleteClient(targetNickname, kickMessage);
			whoChannel(channelName);
		 }
	} else {
		 ft_send(this->_client, ERR_NOSUCHCHANNEL(_client, channelName));
		 return;
	}
}
