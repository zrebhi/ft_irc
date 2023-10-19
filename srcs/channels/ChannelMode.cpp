#include "Channel.hpp"
#include <algorithm>

void Channel::addOperator(Client &user) {
	this->_operators.insert(std::make_pair(user.getNickname(), user));
}

void Channel::removeOperator(Client &user) {
	this->_operators.erase(user.getNickname());
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
		int limitValue = atoi(limitStr.c_str());
		if (limitValue < MIN_CHAN_USERS || limitValue > MAX_CHAN_USERS)
			return; //error bad limits
		else
			_limit = limitValue;
	}
	else
		_limit = 100;
}

void Channel::setTopic(const std::string &channelName, const std::string &name, std::string &content)
{
	// if (_topicLocked || !isOperator(name))
	// 	return; // no rights
	size_t maxTopicLength = 50;
	if (content.length() > maxTopicLength)
	{
		content.resize(maxTopicLength, 0);
		content.append("...");
	}
	_topic = content;

	std::string reply = ":IRC 332 " + name + " " + channelName + " :" + _topic;
	std::cout << reply << std::endl;
	sendMessageToChannel(_users[name], reply);
}

std::string &Channel::getTopic()
{
	return _topic;
}

bool Channel::isTopicLocked()
{
	return _topicLocked;
}

bool Channel::isLimitLocked()
{
	return _limit > 0;
}

int Channel::isFull()
{
	if (_limit < 0)
		return false;
	if ((int)_users.size() >= _limit)
		return true;
	return true;
}

bool Channel::isChannelLocked()
{
	return !_password.empty();
}

bool Channel::isInviteOnly()
{
	return _inviteOnly;
}

bool Channel::checkChannelPassword(const std::string &clientPassword)
{
	if (_password.empty() || clientPassword == _password)
		return true;
	return false;
}

void Channel::setChannelPassword(const std::string &newPassword, const std::string &name, bool addOrRemove)
{
	if (!isOperator(name))
		return;
	if (addOrRemove == false)
		_password = "";
	else if (newPassword.length() < 4) //a gerer
		return; //error + bad pass
	else
	{
		for (size_t i = 0; i < newPassword.length(); i++)
		{
			if (!isalnum(newPassword.at(i)))
				return; //error bad pass
		}
		_password = newPassword;
		std::string reply = ":IRC 324 #" + getName() + " +k New channel password.\n";
		std::map<std::string, Client>::iterator it = this->_users.begin();
		for (; it != _users.end(); ++it)
			send(it->second.getSocket(), reply.c_str(), reply.length(), 0);
	}
}

void Channel::setInvitedList(const std::string &clientName, bool addOrRemove)
{
	if (_users.find(clientName) != _users.end())
		return; //no such client
	if (addOrRemove == ADD)
		_invitedList.push_back(clientName);
	else
	{
        std::vector<std::string>::iterator it = std::find(_invitedList.begin(), _invitedList.end(), clientName);
        if (it != _invitedList.end())
            _invitedList.erase(it);
	}
}
