/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:34:06 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/19 20:12:35 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>
#include <map>
#include <string>

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const std::string &channelName) :
_name(channelName), _inviteOnly(false), _topicLocked(false), _limit(-1) {}

Channel::Channel(const Channel &src) {
	*this = src;
}

Channel &Channel::operator=(const Channel &rhs) {
	this->_users = rhs._users;
	this->_password = rhs._password;
	this->_operators = rhs._operators;
	this->_name = rhs._name;
	this->_limit = rhs._limit;
	this->_inviteOnly = rhs._inviteOnly;
	this->_topicLocked = rhs._topicLocked;
	this->_topic = rhs._topic;

	return (*this);
}

void Channel::addUser(Client &user) {
	this->_users.insert(std::make_pair(user.getNickname(), user));
}

void Channel::removeUser(std::string nickname) {
	this->_users.erase(nickname);
}

void	Channel::userMessageToChannel(Client sender, std::string message) {
	std::map<std::string, Client>::iterator it = this->_users.begin();
	for (; it != _users.end(); ++it) {
		if (it->first != sender.getNickname())
			ft_send(it->second, message);
	}
}

void	Channel::serverMessageToChannel(std::string message) {
	std::map<std::string, Client>::iterator it = this->_users.begin();
	for (; it != _users.end(); ++it) {
			ft_send(it->second, message);
	}
}

bool Channel::isOperator(const std::string &nickname) {
	std::map<std::string, Client>::iterator it = this->_operators.find(nickname);
	if (it != _operators.end())
		return true;
	else
		return false;
}

std::string Channel::getName() const {
	return this->_name;
}

std::map<std::string, Client> &Channel::getUsers() {
	return this->_users;
}

std::string Channel::userListString()
{
	std::string userList;
	std::map<std::string, Client>::iterator it = _users.begin();

	if (!_users.empty()) {
		for (; it != _users.end(); ++it) {
			if (it != _users.begin())
				userList.append(" ");
			if (isOperator(it->second.getNickname()))
				userList.append("@");
			userList.append(it->second.getNickname());
		}
	}
	return userList;
}

bool Channel::isInvited(const std::string &clientName)
{
	std::vector<std::string>::iterator it = std::find(_invitedList.begin(), _invitedList.end(), clientName);
	if (it != _invitedList.end())
	{
		return true;
	}
	return false;
}

void Channel::deleteClient(const std::string &clientName, std::string reply)
{
    std::map<std::string, Client>::iterator clientIt = _users.find(clientName);
    if (clientIt != _users.end())
	{
		_users.erase(clientIt);
		std::map<std::string, Client>::iterator it = _users.begin();
		for (; it != _users.end(); ++it)
			ft_send(it->second, reply);
	}
}

bool Channel::isUserInChannel(const std::string &nickname) const
{
	return _users.find(nickname) != _users.end();
}
