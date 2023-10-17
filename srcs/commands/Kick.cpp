/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:11:30 by bgresse           #+#    #+#             */
/*   Updated: 2023/10/13 12:54:45 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::kick(std::map<std::string, Channel> &channels) {
    if (this->_commandArray.size() < 4) {
        ft_send(this->_client, ":IRC 461 " + _client.getNickname() + " :Not enough parameters");
        return;
    }

    std::string channelName = this->_commandArray[1];

    if (channelName.empty() || channelName[0] != '#') {
        ft_send(this->_client, ":IRC 476 " + _client.getNickname() + " " + channelName + " :Invalid channel mask");
        return;
    }

    std::map<std::string, Channel>::iterator channelIt = channels.find(channelName.substr());
    if (channelIt != channels.end()) {
        Channel &channel = channelIt->second;

        if (!channel.isUserInChannel(_client.getNickname())) {
            ft_send(this->_client, ":IRC 442 " + _client.getNickname() + " " + channelName + " :You're not on that channel");
            return;
        } else if (!channel.isOperator(_client.getNickname())) {
            ft_send(this->_client, ":IRC 482 " + _client.getNickname() + " " + channelName + " :You're not channel operator");
            return;
        } else {
            std::string targetNickname = this->_commandArray[2];

            if (!channel.isUserInChannel(targetNickname)) {
                ft_send(this->_client, ":IRC 401 " + _client.getNickname() + " " + targetNickname + " :No such nick");
                return;
            }

            std::map<std::string, Client> users = channel.getUsers();
            std::map<std::string, Client>::iterator targetUserIt = users.find(targetNickname);
            Client targetUser;
        
            if (targetUserIt != users.end())
                targetUser = targetUserIt->second;
            
            channel.removeUser(targetUser);

            // Optionally, you can notify the target client that they were kicked
            ft_send(channel.getUsers()[targetNickname], ":KICK " + channelName + " " + _client.getNickname());
        }
    } else {
        ft_send(this->_client, ":IRC 403 " + _client.getNickname() + " " + channelName + " :No such channel");
        return;
    }
}
