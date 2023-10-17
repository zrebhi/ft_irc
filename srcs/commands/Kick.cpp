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
	std::map<std::string, Channel> &channels = this->_ircServ.getChannelList();

    if (this->_commandArray.size() < 4) {
        ft_send(this->_client, ":IRC 461 KICK :Not enough parameters");
        return;
    }

    std::string channelName = this->_commandArray[1];
    ft_send(this->_client, channelName);

    if (channelName.empty() || channelName[0] != '#') {
        ft_send(this->_client, ":IRC 476 " + channelName + " :Bad Channel Mask");
        return;
    }

    std::map<std::string, Channel>::iterator channelIt = channels.find(channelName.substr(1));
    if (channelIt != channels.end()) {
        Channel &channel = channelIt->second;

        if (!channel.isUserInChannel(_client.getNickname())) {
            ft_send(this->_client, ":IRC 442 " + channelName + " :You're not on that channel");
            return;
        } else if (!channel.isOperator(_client.getNickname())) {
            ft_send(this->_client, ":IRC 482 " + channelName + " :You're not channel operator");
            return;
        } else {
            std::string targetNickname = this->_commandArray[2];

            if (!channel.isUserInChannel(targetNickname)) {
                ft_send(this->_client, ":IRC 401 " + targetNickname + " :No such nick/channel");
                return;
            }

            std::map<std::string, Client> users = channel.getUsers();
            std::map<std::string, Client>::iterator targetUserIt = users.find(targetNickname);
            Client targetUser;

            if (targetUserIt != users.end())
                targetUser = targetUserIt->second;

            ft_send(channel.getUsers()[targetNickname], "you have been kicked from " + channelName + " by " + _client.getNickname());
            whoChannel();
			std::string reply = _client.getNickname() + " has kicked " + targetNickname + " from " + channelName;
            channel.deleteClient(targetNickname, reply);
        }
    } else {
        ft_send(this->_client, ":IRC 403 " + channelName + " :No such channel");
        return;
    }
}