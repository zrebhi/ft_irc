#include "../inc/Channels.hpp"
#include <string>
#include <vector>

Channels::Channels()
	: _chanName(""), _chanMode(1), _isProtected(false), _topic("No topic defined\n")
{
}

Channels::Channels(std::string chanName)
	: _chanName(chanName), _chanMode(1), _isProtected(false), _topic("No topic defined\n")
{
}

Channels::Channels(std::string chanName, Client *client)
	: _chanName(chanName), _chanMode(1), _isProtected(false), _topic("No topic defined\n")
{
	_operators.push_back(client);
}

Channels::~Channels()
{
}

std::string Channels::getTopic()
{
	return _topic;
}

int Channels::getMode()
{
	return _chanMode;
}

std::string Channels::getChanName()
{
	return _chanName;
}

std::string Channels::getNbUsers()
{
	std::stringstream size;
	size << _usersList.size();
	return size.str();
}

int Channels::checkProtected(const std::string &password)
{
	if (_isProtected == false)
		return -1;
	if (password == _password)
		return 1;
	else
		return 0;
}

std::string Channels::getusersList()
{
	std::vector<Client *>::iterator it = _operators.begin();
	std::string usersListStr;
	while (it != _operators.end())
	{
		if (usersListStr.empty() == false)
			usersListStr.append(" ");
		usersListStr.append((*it)->getNick());
		it++;
	}
	it = _usersList.begin();
	while (it != _usersList.end())
	{
		if (usersListStr.empty() == false)
			usersListStr.append(" ");
		usersListStr.append((*it)->getNick());
		it++;
	}
	usersListStr.append("\n");
	return usersListStr;
}

int Channels::addUser(Client *newClient)
{
	std::string added = ":" + newClient->getNick() + "@localhost JOIN :" + getChanName() + "\n";
	if (_chanMode > 42) // invite only
	{
		std::vector<Client *>::iterator it = _invited.begin();
		while (it != _invited.end())
		{
			if (*it == newClient)
			{
				_usersList.push_back(newClient);
				send(newClient->getSocket(), added.c_str(), added.size(), 0);
				return 1;
			}
			it++;
		}
	}
	else
	{
		_usersList.push_back(newClient);
		for (std::vector<Client *>::iterator it = _usersList.begin(); it != _usersList.end(); it++)
			send((*it)->getSocket(), added.c_str(), added.size(), 0);
		return 1;
	}
	return 0;
}

bool Channels::removeUser(Client *oldClient)
{
	std::vector<Client *>::iterator it = _usersList.begin();
	while (it != _usersList.end())
	{
		if (oldClient->getNick().compare((*it)->getNick()))
		{
			_usersList.erase(it);
			if (_usersList.empty())
				return false;
			return true;
		}
		it++;
	}
	return true;
}

void Channels::setTopic(std::string newTopic)
{
	_topic = newTopic;
}

void Channels::setChanName(std::string newChanName)
{
	_chanName = newChanName;
}

void Channels::setMode(int mode)
{
	_chanMode = mode;
}

void Channels::invite(std::string nickname)
{
	(void)nickname;
	// todo
}
