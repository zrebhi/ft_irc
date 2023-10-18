/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:38:38 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 22:40:08 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include "../server/Server.hpp"

class Client;

class Channel {
public:
	Channel();
	~Channel();
	Channel(const std::string &channelName);
	Channel(const Channel &src);
	Channel	&operator=(const Channel &rhs);


	void	addUser(Client &user);
	void	addOperator(Client &user);
	void removeOperator(Client &user);

	void	sendMessageToChannel(Client sender, std::string message);
	bool	isUserInChannel(const std::string &nickname);
// Mode related funcs
	bool	checkChannelPassword(const std::string &password);
	void setChannelPassword(const std::string &password, const std::string &name, bool addOrRemove);
	bool isChannelLocked();
	void setTopicLock(bool addOrRemove, const std::string &name);
	void setLimit(bool addOrRemove, const std::string &name, std::string limit);
	void setTopic(const std::string &name, std::string &content);
	bool isTopicLocked();
	int isFull();
	bool isInviteOnly();
	bool isInvited(const std::string &name);
	void setInvitedList(const std::string &clientName, bool addOrRemove);
	void setInviteOnly(bool addOrRemove, const std::string &name);
	bool isLimitLocked();
//
	void deleteClient(const std::string &clientName, std::string &message);
// 
	bool	isOperator(const std::string &nickname);

	std::string	getName() const;
	std::map<std::string, Client> &getUsers();
	std::string &getTopic();
	std::string	userListString();

private:
	std::string	_name;
	std::string _password;
	bool _inviteOnly;
	bool _topicLocked;
	int _limit;
	std::vector<std::string> _invitedList;
	std::string _topic;
	std::map<std::string, Client>	_users;
	std::map<std::string, Client>	_operators;
};
