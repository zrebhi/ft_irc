/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:38:38 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/17 12:27:04 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include "../client/Client.hpp"

class Client;

class Channel {
public:
	Channel();
	Channel(const std::string &channelName);
	~Channel();

	void	addUser(Client &user);
	void	removeUser(Client &user);
	void	addOperator(Client &user);
	void removeOperator(Client &user);

	void	sendMessageToChannel(Client sender, std::string message);
	bool checkPassword(const std::string &password);
	void setPassword(const std::string &password, const std::string &name, bool addOrRemove);
	void setInviteOnly(bool addOrRemove, const std::string &name);
	void setTopicLock(bool addOrRemove, const std::string &name);
	void setLimit(bool addOrRemove, const std::string &name, std::string limit);
	void setTopic(const std::string &name, std::string &content);

	bool	isOperator(const std::string &nickname);
	bool	isUserInChannel(const std::string &nickname) const;

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
	std::string _topic;
	std::map<std::string, Client>	_users;
	std::map<std::string, Client>	_operators;
	// std::map<std::string, Client&>	_invited;
};
