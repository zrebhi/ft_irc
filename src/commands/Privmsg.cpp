/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 21:40:45 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/04 23:31:36 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::privmsg(Server ircServ, Client user) {
	if (this->_commandArray[1][0] == '#')
		channelMessage(ircServ, user);
}

void Command::channelMessage(Server ircServ, Client user) {
	if (!checkChannelExists(ircServ, user))
		return ;
	if (!checkIsChannelMember(ircServ, user))
		return ;

	std::string reply = ":" + user.getNickname() + " ";
	for (size_t i = 0; i < this->_commandArray.size(); i++) {
		reply.append(this->_commandArray[i]);
		if (i < this->_commandArray.size() - 1)
			reply.append(" ");
	}
// Send message to every member of the channel, except the sender.
	std::map<int, Client>	clientList = ircServ.getClientList();
	std::map<int, Client>::iterator it = clientList.begin();
	for (; it != clientList.end(); it++) {
		if (it->first != user.getSocket())
			ft_send(it->second, reply);
	}
}

bool Command::checkChannelExists(Server ircServ, Client user) {
	std::string channelName = this->_commandArray[1].substr(1);

	std::map<std::string, Channel> channelList = ircServ.getChannelList();
	std::map<std::string, Channel>::iterator it = channelList.find(channelName);
	if (it == channelList.end()) {
		std::string reply = ":IRC 403 " + user.getUsername() + " " + channelName + " :No such channel";
		ft_send(user, reply);
		return false;
	}
	else
		return true;
}

bool Command::checkIsChannelMember(Server ircServ, Client user) {
	std::string channelName = this->_commandArray[1].substr(1);
	std::map<std::string, Channel> channelList = ircServ.getChannelList();

	std::map<int, Client> channelUsers = channelList[channelName].getUserList();
	std::map<int, Client>::iterator it = channelUsers.find(user.getSocket());
	if (it == channelUsers.end()) {
		std::string reply = ":IRC 442 " + user.getUsername() + " " + channelName + " :You're not on that channel";
		ft_send(user, reply);
		return false;
	}
	else
		return true;
}