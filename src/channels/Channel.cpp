/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:34:06 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/10 00:00:09 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string &channelName) : _name(channelName) {}

Channel::~Channel() {}

void Channel::addUser(Client &user) {
	this->_users.insert(std::make_pair(user.getSocket(), user));
}

void Channel::addOperator(Client &user) {
	this->_operators.insert(std::make_pair(user.getSocket(), user));
}

std::string Channel::getName() const {
	return this->_name;
}

std::map<int, Client> Channel::getUsers() {
	return this->_users;
}

std::string Channel::getUserList() {
	std::string userList;
	std::map<int, Client>::iterator it_op;
	std::map<int, Client>::iterator it = _users.begin();

	if (!_users.empty()) {
		for (; it != _users.end(); ++it) {
			it_op = _operators.find(it->first);
			if (it_op != _operators.end())
				userList.append("@");
			userList.append(it->second.getNickname());
			userList.append(" ");
		}
		userList.erase(userList.size() - 1); // Removes the space at the end.
	}
	return userList;
}
