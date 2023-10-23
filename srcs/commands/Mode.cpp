#include "Command.hpp"
#include <cctype>

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

std::string Command::currentModesStr()
{
	std::string channelName = _commandArray[1];
	if (!channelExists(channelName))
		return ERR_NOSUCHCHANNEL(this->_client, channelName);
	Channel &channel = _ircServ.getChannel(channelName);
	std::string reply = RPL_CHANNELMODEIS(_client, channel);

	if (channel.isTopicLocked())
		reply.append("t");
	if (channel.isChannelLocked())
		reply.append("k");
	if (channel.isInviteOnly())
		reply.append("i");
	if (channel.isLimitLocked())
	{
		reply.append("l - limited to ");
		reply.append(intToString(channel.isLimitLocked()));
		reply.append(" users");

	}
	return reply;
}

void Command::setITKL_Modes(char letterMode, size_t &argIndex)
{
	bool addOrRemoveMode;
	std::string channelName = _commandArray[1];
	Channel &channel = _ircServ.getChannel(channelName);
	std::string modes = _commandArray[2]; 

	addOrRemoveMode = (modes.at(0) == '+') ? ADD : REMOVE;
	if (letterMode == 'i')
		channel.setInviteOnly(addOrRemoveMode, _client.getNickname());
	else if (letterMode == 't')
		channel.setTopicLock(addOrRemoveMode, _client.getNickname());
	else if (letterMode == 'l')
	{
		if (addOrRemoveMode == ADD && argIndex < _commandArray.size())
			channel.setLimit(addOrRemoveMode, _client, _commandArray[argIndex++]);
		else
			channel.setLimit(addOrRemoveMode, _client, "empty");
	}
	else if (letterMode == 'k')
	{
		std::string password = "";
		if (addOrRemoveMode == ADD && _commandArray.size() < 4)
			return ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "MODE"));
		if (_commandArray.size() && argIndex < _commandArray.size())
			password = _commandArray[argIndex++];
		channel.setChannelPassword(password, _client.getNickname(), addOrRemoveMode);
	}
}

void Command::setO_Modes(size_t &argIndex)
{
	bool addOrRemoveMode;
	std::string channelName = _commandArray[1];
	Channel &channel = _ircServ.getChannel(channelName);
	std::string modes = _commandArray[2]; 
	std::map<std::string, Client> &users = channel.getUsers();
	if (argIndex >= _commandArray.size())
		return ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "MODE"));
	std::string argument = _commandArray[argIndex];

	addOrRemoveMode = (modes.at(0) == '+') ? ADD : REMOVE;
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
		argIndex++;
	}
	else
		return ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "MODE"));
}

void	Command::mode()
{
	int modeHasChange = 0;
	if (_commandArray.size() == 2)
		return ft_send(_client, currentModesStr());
	if (_commandArray.size() < 2)
		return ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "MODE"));
	std::string itkolModes = "itkol";
	std::string channelName = _commandArray[1];
	std::string modes = _commandArray[2]; 
	if (channelName.empty() || channelName.at(0) != '#')
		return ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	channelName = channelName.substr(1);
	if (!channelExists(channelName))
		return ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	Channel &channel = _ircServ.getChannel(channelName);
	if (!channel.isOperator(_client.getNickname()))
		return ft_send(this->_client, ERR_CHANOPRIVSNEEDED(channelName, _client));
	if (channel.getUsers().find(_client.getNickname()) == channel.getUsers().end())
		return ft_send(this->_client, ERR_NOTONCHANNEL(channelName));
	size_t argIndex = 3;
	for (size_t i = 1; i < modes.length(); i++)
	{
		if (modes.at(i) == 'o')
			this->setO_Modes(argIndex);
		if (strchr("itkol", modes.at(i)) && ++modeHasChange)
			this->setITKL_Modes(modes.at(i), argIndex);
		else
			ft_send(_client, ERR_UNKNOWNMODE(modes.substr(i, 1), channelName));
	}
	if (modeHasChange)
		channel.serverMessageToChannel(currentModesStr());
}
