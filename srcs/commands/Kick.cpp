/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/17 19:51:53 by bgresse           #+#    #+#             */
/*   Updated: 2023/10/17 20:28:23 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::kick() {
	std::cout << "enter kick" << std::endl;
	for(size_t i = 0 ; i < _commandArray.size(); i++)
		std::cout << "\t-> " << _commandArray[i] << std::endl;
    if (this->_commandArray.size() < 3)
		return ft_send(this->_client, ERR_NEEDMOREPARAMS(_client, "KICK"));
	std::map<std::string, Channel> &channels = this->_ircServ.getChannelList();
    std::string channelName = this->_commandArray[1];

    if (channelName.empty() || channelName[0] != '#')
        return ft_send(this->_client, ERR_BADCHANNELMASK(channelName));

    std::map<std::string, Channel>::iterator channelIt = channels.find(channelName.substr(1));
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
			std::cout << "\treply = " << kickMessage << std::endl;
            channel.deleteClient(targetNickname, kickMessage);
        }
    } else {
        ft_send(this->_client, ERR_NOSUCHCHANNEL(_client, channelName));
        return;
    }
}
