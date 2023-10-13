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
	for (int i = 1; i < inputModes.length(); i++)
	{
		if (modesToHandle.find(inputModes.at(i)))
			return false;
		modesToHandle.erase(i);
	}
	return true;
}

void	Command::mode(std::map <std::string, Channel> &channels)
{
	//gestion uniquement du mot de passe
	bool addOrRemoveMode;
	std::string channelName = _commandArray[1];
	std::string modes = _commandArray[2];
	std::string argument = "";
	if (channelName.at(0) != '#' || !isValidModes(modes))
		return; //error + usage
	else
		channelName = channelName.substr(1);
	if (!channelExists(channelName))
		return; // no such chan
	if (_commandArray.size() == 4)
		argument = _commandArray[3];
	Channel &channel = channels[channelName];
	addOrRemoveMode = modes.at(0) == '+' ? ADD : REMOVE;
	for (int i = 1; i < modes.length(); i++)
	{
		if (modes.at(i) == 'k')
		{
			if (addOrRemoveMode == ADD && _commandArray.size() < 4)
				return; //error + need param + usage
			std::string password = _commandArray[3];
			if (!channelExists(channelName) || password.empty())
				return; //error + no such channel
			channel.setPassword(password, _client.getNickname()/* , addOrRemoveMode */);
		}
		else if (modes.at(i) == 'i')
		{
			if (channel.isOperator(_client.getNickname()))
				channel.setInviteOnly(addOrRemoveMode);
			else
				return; //not operator
		}
		else if (modes.at(i) == 't')
		{
			if (channel.isOperator(_client.getNickname()))
				channel.setTopicLock(addOrRemoveMode);
			else
				return; //not operator
		}
		else if (modes.at(i) == 'o')
		{
			std::map<std::string, Client> &users = channel.getUsers();
			if (!argument.empty() && users.find(argument) != users.end())
					channel.addOperator(users.find(argument)->second);
			else
				return; //no such user
		}
		else if (modes.at(i) == 'l')
		{
			if (channel.isOperator(_client.getNickname()))
				channel.setLimit(addOrRemoveMode, argument);
			else
				return; //not operator
		}
	}
}
