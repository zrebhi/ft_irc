/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:11:30 by bgresse           #+#    #+#             */
/*   Updated: 2023/10/13 11:29:16 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::kick(std::map<std::string, Channel> &channels)
{
    if (this->_commandArray.size() < 4)
    {
        ft_send(this->_client, ":IRC 461 " + _client.getNickname() + " :Not enough parameters");
        return;
    }
    
    std::string channelName = this->_commandArray[1];

    if (channelName.empty() || channelName[0] != '#')
    {
        ft_send(this->_client, ":IRC 476 " + _client.getNickname() + " " + channelName + " :Invalid channel mask");
        return;
    }

    std::map<std::string, Channel>::iterator channelIt = channels.find(channelName);
    if (channelIt != channels.end())
    {
        Channel &channel = channelIt->second;

        if (!channel.isUserInChannel(_client.getNickname()))
        {
            ft_send(this->_client, ":IRC 442 " + _client.getNickname() + " " + channelName + " :You're not on that channel");
            return;
        }
        else if (!channel.isOperator(_client.getNickname()))
        {
            ft_send(this->_client, ":IRC 482 " + _client.getNickname() + " " + channelName + " :You're not channel operator");
            return;
        }
        else
        {
            // Client targetClient = findClient(this->_commandArray[2]);
            // kick(targetClient, channel);
        }
    }
    else
    {
        ft_send(this->_client, ":IRC 403 " + _client.getNickname() + " " + channelName + " :No such channel");
        return;
    }
}
