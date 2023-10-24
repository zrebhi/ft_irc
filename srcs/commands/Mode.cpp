#include "Command.hpp"

void	Command::mode()
{
	if (_commandArray.size() < 2 || _commandArray[1].empty())
		return ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "MODE"));

	std::string channelName = _commandArray[1];
	if (!validChannelName(channelName))
		return ft_send(this->_client, ERR_INVALIDCHANNEL(_client, channelName));
	channelName = formatChannelName(channelName);
	if (!channelExists(channelName))
		return ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	if (_commandArray.size() == 2 || _commandArray[2].empty())
		return ft_send(_client, currentModesStr(channelName));

	std::string modes = _commandArray[2];
	Channel &channel = _ircServ.getChannel(channelName);
	if (!channel.isOperator(_client.getNickname()))
		return ft_send(this->_client, ERR_CHANOPRIVSNEEDED(channelName, _client));
	if (channel.getUsers().find(_client.getNickname()) == channel.getUsers().end())
		return ft_send(this->_client, ERR_NOTONCHANNEL(channelName));

	int modeHasChange = 0;
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
		channel.serverMessageToChannel(currentModesStr(channelName));
}

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

std::string Command::currentModesStr(std::string channelName)
{
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

	if (modes.size() < 2)
		return ft_send(_client, ERR_NEEDMOREPARAMS(_client, "MODE"));
	if (modes.at(0) == '+')
		addOrRemoveMode = ADD;
	else if (modes.at(0) == '-')
		addOrRemoveMode = REMOVE;
	else
		return;
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

	if (modes.size() < 2)
		return ft_send(_client, ERR_NEEDMOREPARAMS(_client, "MODE"));
	if (modes.at(0) == '+')
		addOrRemoveMode = ADD;
	else if (modes.at(0) == '-')
		addOrRemoveMode = REMOVE;
	else
		return;
	if (!argument.empty() && users.find(argument) != users.end())
	{
		if ((addOrRemoveMode == ADD && channel.isOperator(argument)) || \
		(addOrRemoveMode == REMOVE && !channel.isOperator(argument)))
			return;
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
