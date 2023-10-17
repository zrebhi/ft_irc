/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:34:06 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/17 12:26:28 by bgresse          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <cstdlib>

#define ADD true
#define REMOVE false
#define MAX_CHAN_USERS 100
#define MIN_CHAN_USERS 3

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

void Channel::removeOperator(Client &user) {
	this->_operators.erase(user.getNickname());
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

bool Channel::checkPassword(const std::string &clientPassword)
{
	if (_password.empty())
		return true;
	if (clientPassword == _password)
		return true;
	return false;
}

void Channel::setPassword(const std::string &newPassword, const std::string &name, bool addOrRemove)
{
	if (!isOperator(name))
		return; //error + not right
	if (addOrRemove == false)
		_password = "";
	else if (newPassword.length() < 4) //a gerer
		return; //error + bad pass
	else
	{
		std::string::const_iterator it = newPassword.begin();
		while (it != newPassword.end())
		{
			if (!isalnum(*it++))
				return; //error bad pass
		}
		_password = newPassword;
		// send ok changing pass
	}
}

void Channel::setInviteOnly(bool addOrRemove,  const std::string &name)
{
	if (!isOperator(name))
		return; //error + not right
	_inviteOnly = addOrRemove;
}

void Channel::setTopicLock(bool addOrRemove, const std::string &name)
{
	if (!isOperator(name))
		return; //error + not right
	_topicLocked = addOrRemove;
}

void Channel::setLimit(bool addOrRemove, const std::string &name, std::string limitStr)
{
	if (!isOperator(name))
		return; //error + not right
	if (addOrRemove == ADD)
	{
		int limitValue = stringToInt(limitStr);
		if (limitValue < MIN_CHAN_USERS || limitValue > MAX_CHAN_USERS)
			return; //error bad limits
		else
			_limit = limitValue;
	}
	else
		_limit = 100;
}

void Channel::setTopic(const std::string &name, std::string &content)
{
	if (_topicLocked || !isOperator(name))
		return; // no rights
	size_t maxTopicLength = 50;
	if (content.length() > maxTopicLength)
	{
		content.resize(maxTopicLength, 0);
		content.append("...");
	}
	_topic = content;
	std::string reply = "332 " + getName() + " :" + _topic;
	sendMessageToChannel(_users[name], reply);
}

std::string &Channel::getTopic()
{
	return _topic;
}
