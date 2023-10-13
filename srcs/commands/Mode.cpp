#include "Command.hpp"
#include <map>
#include <string>

#define ADD true
#define REMOVE false

bool isValidModes(std::string &inputModes)
{
	std::string modesToHandle = "itkol";
	if (inputModes.at(0) != '+' && inputModes.at(0) != '-')
		return false;
	for (size_t i = 1; i < inputModes.length(); i++)
	{
		if (!modesToHandle.find(inputModes.at(i)))
			return false;
		modesToHandle.erase(i, 1);
	}
	return true;
}

void	Command::mode(std::map <std::string, Channel> &channels)
{
	bool addOrRemoveMode;
	std::string channelName = _commandArray[1];
	if (!channelExists(channelName) || channelName.at(0) != '#')
		return; // no such chan
	if (_commandArray.size() == 2)
		return;
	std::string modes = _commandArray[2];
	std::string argument = "";
	if (!isValidModes(modes))
		return; //error + usage
	else
		channelName = channelName.substr(1);
	if (_commandArray.size() == 4)
		argument = _commandArray[3];
	Channel &channel = channels[channelName];
	addOrRemoveMode = (modes.at(0) == '+') ? ADD : REMOVE;
	for (size_t i = 1; i < modes.length(); i++)
	{
		if (modes.at(i) == 'i')
			channel.setInviteOnly(addOrRemoveMode, _client.getNickname());
		else if (modes.at(i) == 't')
				channel.setTopicLock(addOrRemoveMode, _client.getNickname());
		else if (modes.at(i) == 'l')
				channel.setLimit(addOrRemoveMode, _client.getNickname(), argument);
		else if (modes.at(i) == 'k')
		{
			if (addOrRemoveMode == ADD && _commandArray.size() < 4)
				return; //error + need param + usage
			std::string password = _commandArray[3];
			channel.setPassword(password, _client.getNickname(), addOrRemoveMode);
		}
		else if (modes.at(i) == 'o')
		{
			std::map<std::string, Client> &users = channel.getUsers();
			if (!argument.empty() && users.find(argument) != users.end())
			{
				if (addOrRemoveMode == ADD)
					channel.addOperator(users.find(argument)->second);
				if (addOrRemoveMode == REMOVE)
					channel.removeOperator(users.find(argument)->second);
				std::string reply = ":" + _client.getNickname() + "!" + _client.getUsername() + "@localhost MODE #" + channel.getName() + " " + modes.at(0) + "o " + users.find(argument)->second.getNickname();
				std::map<std::string, Client>::iterator it = channel.getUsers().begin();
				for (; it != channel.getUsers().end(); it++)
					ft_send(it->second, reply);
			}

			else
				return; //no such user
		}
	}
}
