/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:34:06 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/03 21:33:22 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(const std::string &channelName) : _name(channelName) {}

Channel::~Channel() {}

void Channel::addUser(const Client &user) {
	this->_users.insert(std::make_pair(user.getSocket(), user));
	std::cout << this->_users[user.getSocket()].getNickname() << " added to " << this->_name << std::endl;
}

void Channel::printUserList() {
	std::map<int, Client>::iterator it = this->_users.begin();

	for (; it != this->_users.end(); it++)
		std::cout << it->second.getNickname() << std::endl;
}

std::string Channel::getName() const {
	return this->_name;
}

std::map<int, Client> Channel::getUserList() const {
	return this->_users;
}