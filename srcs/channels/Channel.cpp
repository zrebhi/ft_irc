/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:34:06 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 23:17:59 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const std::string &channelName) : _name(channelName) {}

Channel::Channel(const Channel &src) {
	*this = src;
}

Channel &Channel::operator=(const Channel &rhs) {
	this->_users = rhs._users;
	this->_password = rhs._password;
	this->_operators = rhs._operators;
	this->_name = rhs._name;

	return (*this);
}

void Channel::addUser(Client &user) {
	this->_users.insert(std::make_pair(user.getNickname(), user));
}

void Channel::addOperator(Client &user) {
	this->_operators.insert(std::make_pair(user.getNickname(), user));
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

std::string Channel::getName() const {
	return this->_name;
}

std::map<std::string, Client> Channel::getUsers() {
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

bool Channel::checkPassword(const std::string &clientPassword)
{
	if (_password.empty() || clientPassword == _password)
		return true;
	return false;
}

void Channel::setPassword(const std::string &newPassword, const std::string &name)
{
	if (newPassword.length() < 2) //a gerer
		return;
	std::string::const_iterator it = newPassword.begin();
	while (it != newPassword.end())
	{
		if (!isalnum(*it++))
			return;
	}
	if (_password == "" || !isOperator(name))
	{
		_password = newPassword;
		// send ok changing pass
	}
}
