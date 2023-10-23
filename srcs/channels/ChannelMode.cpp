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
		return;
	_inviteOnly = addOrRemove;
}

void Channel::setTopicLock(bool addOrRemove, const std::string &name)
{
	if (!isOperator(name))
		return;
	_topicLocked = addOrRemove;
}

void Channel::setLimit(bool addOrRemove, Client &client, std::string limitStr)
{
	if (addOrRemove == ADD)
	{
		int limitValue = atoi(limitStr.c_str());
		if (!isValidNumber(limitStr) || limitValue < MIN_CHAN_USERS || limitValue > MAX_CHAN_USERS)
			ft_send(client, ERR_BADLIMIT(client, getName(), limitStr));
		else
			_limit = limitValue;
	}
	else
		_limit = MAX_CHAN_USERS;
}

void Channel::setTopic(const std::string &clientName, std::string &content)
{
	if (content.length() > MAX_TOPIC_LENGTH)
	{
		content.resize(MAX_TOPIC_LENGTH, 0);
		content.append("...");
	}
	_topic = content;
	
	serverMessageToChannel(RPL_TOPIC(_users[clientName], getName(), _topic));

	if (_topic.empty())
		ft_send(_users[clientName], ":IRC 331 " + clientName + " #" + this->getName() + " :No topic set.");
}

std::string &Channel::getTopic()
{
	return _topic;
}

bool Channel::isTopicLocked()
{
	return _topicLocked;
}

int Channel::isLimitLocked()
{
	if (_limit != MAX_CHAN_USERS)
		return _limit;
	return 0;
}

int Channel::isFull()
{
	if (_users.size() >= _limit)
		return true;
	return false;
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
	else
	{
		for (size_t i = 0; i < newPassword.length(); i++)
		{
			if (!isalnum(newPassword.at(i)))
				return;
		}
		_password = newPassword;
	}
}

void Channel::setInvitedList(const std::string &clientName, bool addOrRemove)
{
	if (_users.find(clientName) != _users.end())
		return;
	if (addOrRemove == ADD)
		_invitedList.push_back(clientName);
	else
	{
        std::vector<std::string>::iterator it = std::find(_invitedList.begin(), _invitedList.end(), clientName);
        if (it != _invitedList.end())
            _invitedList.erase(it);
	}
}
