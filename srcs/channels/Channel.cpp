/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:34:06 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/17 20:25:50 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <algorithm>
#include <map>
#include <string>

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const std::string &channelName) : _name(channelName), _limit(-1) {}

Channel::Channel(const Channel &src) {
	*this = src;
}

Channel &Channel::operator=(const Channel &rhs) {
	this->_users = rhs._users;
	this->_password = rhs._password;
	this->_operators = rhs._operators;
	this->_name = rhs._name;
	this->_limit = rhs._limit;

	return (*this);
}

void Channel::addUser(Client &user) {
	this->_users.insert(std::make_pair(user.getNickname(), user));
}


void	Channel::sendMessageToChannel(Client sender, std::string message) {
	std::map<std::string, Client>::iterator it = this->_users.begin();
	for (; it != _users.end(); ++it) {
		if (it->first != sender.getNickname())
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

bool Channel::isUserInChannel(const std::string &nickname) const
{
    return _users.find(nickname) != _users.end();
}

std::string Channel::getName() const {
	return this->_name;
}

std::map<std::string, Client> &Channel::getUsers() {
	return this->_users;
}

std::string Channel::userListString() {
	std::string userList;
	std::map<std::string, Client>::iterator it = _users.begin();

	if (!_users.empty()) {
		for (; it != _users.end(); ++it) {
			if (isOperator(it->second.getNickname()))
				userList.append("@");
			userList.append(it->second.getNickname());
			userList.append(" ");
		}
		userList.erase(userList.size() - 1); // Removes the space at the end.
	}
	return userList;
}

bool Channel::isInvited(const std::string &clientName)
{
	std::vector<std::string>::iterator it = std::find(_invitedList.begin(), _invitedList.end(), clientName);
	if (it != _invitedList.end())
	{
		std::cout << "is invited" << std::endl;
		return true;
	}
	std::cout << "is not invited" << std::endl;
	return false;
}

void Channel::deleteClient(const std::string &clientName, std::string &reply)
{
    std::map<std::string, Client>::iterator clientIt = _users.find(clientName);
    if (clientIt != _users.end())
	{
		std::map<std::string, Client>::iterator it = _users.begin();
		for (; it != _users.end(); ++it)
		{
			if (send(it->second.getSocket(), reply.c_str(), reply.length(), 0) < 0)
				std::cout << "Failed to send message" << std::endl;
		}
        _users.erase(clientIt);
	}
}
