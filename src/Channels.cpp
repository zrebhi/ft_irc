#include "../inc/Channels.hpp"
#include <string>

Channels::Channels()
{
}

Channels::Channels(std::string chanName)
	: _chanName(chanName)
{
	_chanMode = 42;
}

Channels::~Channels()
{
}

std::string Channels::getTopic()
{
	return _topic;
}
std::string Channels::getChanName()
{
	return _chanName;
}
std::string Channels::getusersList()
{
	// todo
	return "todo";
}
int Channels::addUser(Client *newClient)
{
	std::string added = "user added in channel\n";
	_usersList.push_back(newClient);
	send(newClient->getSocket(), added.c_str(), added.size(), 0);
	return 1;
	// if (_chanMode < 1)
	// {
	// }
	// else
	// {
	// 	std::cout << "can't add user" << std::endl;
	// 	return -1;
	// }
}

bool Channels::removeUser(Client *oldClient)
{
	std::vector<Client *>::iterator it = _usersList.begin();
	while (it != _usersList.end())
	{
		if (oldClient->getNickname().compare((*it)->getNickname()))
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
