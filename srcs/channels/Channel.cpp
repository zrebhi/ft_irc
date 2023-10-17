/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:34:06 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/17 19:29:24 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string &channelName) : _name(channelName) {}

Channel::~Channel() {}

void Channel::addUser(Client &user) {
	this->_users.insert(std::make_pair(user.getNickname(), user));
}
void Channel::removeUser(Client &user) {
	
    std::map<std::string, Client>::iterator it = _users.find(user.getNickname());
    if (it != _users.end())
        _users.erase(it);
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

bool Channel::isUserInChannel(const std::string &nickname) const
{
    return _users.find(nickname) != _users.end();
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
