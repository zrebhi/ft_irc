/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ubuntu <ubuntu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 19:51:53 by bgresse           #+#    #+#             */
/*   Updated: 2023/10/23 18:53:12 by ubuntu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::kick() {
	if (_commandArray.size() < 3 || _commandArray[1].empty() || _commandArray[2].empty())
		return ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, _commandArray[0]));

	if (_commandArray[1] == "IRC") {
		_commandArray.erase(_commandArray.begin() + 1);
		_commandArray[2] = _commandArray[2].substr(1);
	}
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
		 if (!channel.isOperator(_client.getNickname()))
			  return ft_send(this->_client, ERR_CHANOPRIVSNEEDED(channelName, _client));

		  std::string targetNickname = this->_commandArray[2];

		if (!channel.isUserInChannel(targetNickname))
				return ft_send(this->_client, ERR_NOSUCHNICKCHAN(_client, channelName, targetNickname, "KICK"));

		  std::string reason;
		  if (_commandArray.size() >= 4) {
			  for (size_t i = 3; i < this->_commandArray.size(); i++) {
				  reason.append(this->_commandArray[i]);
				  if (i < this->_commandArray.size() - 1)
				 	 reason.append(" ");
			  }
		  }


		std::string kickMessage = RPL_KICK(_client.getNickname(), channelName, targetNickname, reason);
		channel.deleteClient(targetNickname, kickMessage);
	} else {
		 ft_send(this->_client, ERR_NOSUCHCHANNEL(_client, channelName));
		 return;
	}
}
