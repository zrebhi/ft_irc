#include "Command.hpp"

void	Command::mode()
{
	if (_commandArray.size() == 2 && !_commandArray[1].empty())
		return currentModesStr();
	if (_commandArray.size() < 3 || _commandArray[1].empty() || _commandArray[2].empty())
		return ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, _commandArray[0]));

	std::string itkolModes = "itkol";
	std::string channelName = _commandArray[1];
	std::string modes = _commandArray[2];
	if (channelName.empty() || channelName.at(0) != '#')
		return (void)ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	channelName = channelName.substr(1);
	if (!channelExists(channelName))
		return (void)ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	Channel &channel = _ircServ.getChannel(channelName);
	if (!channel.isOperator(_client.getNickname()))
		return (void)ft_send(this->_client, ERR_CHANOPRIVSNEEDED(channelName));
	if (channel.getUsers().find(_client.getNickname()) == channel.getUsers().end())
		return (void)ft_send(this->_client, ERR_NOTONCHANNEL(channelName));
	size_t argIndex = 3;
	for (size_t i = 1; i < modes.length(); i++)
	{
		if (modes.at(i) == 'o')
			this->setO_Modes(argIndex);
		if (modes[i] == 'i' || modes[i] == 't' || modes[i] == 'k' || modes[i] == 'l')
			this->setITKL_Modes(modes.at(i), argIndex);
		else
			ft_send(_client, ERR_UNKNOWNMODE(modes.substr(i, 1)));
	}
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

void Command::currentModesStr()
{
	std::string channelName = _commandArray[1];
	if (!channelExists(channelName))
		ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	Channel &channel = _ircServ.getChannel(channelName);
	std::string reply = RPL_CHANNELMODEIS(_client, channel);

	if (channel.isTopicLocked())
		reply.append("t");
	if (channel.isLimitLocked())
		reply.append("l");
	if (channel.isChannelLocked())
		reply.append("k");
	if (channel.isInviteOnly())
		reply.append("i");
	ft_send(this->_client, reply);
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
		channel.setLimit(addOrRemoveMode, _client.getNickname(), _commandArray[argIndex++]);
	else if (letterMode == 'k')
	{
		if (addOrRemoveMode == ADD && _commandArray.size() < 4)
			return (void)ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "MODE"));
		std::string password = _commandArray[3];
		channel.setChannelPassword(password, _client.getNickname(), addOrRemoveMode);
	}
	currentModesStr();
}

void Command::setO_Modes(size_t &argIndex)
{
	bool addOrRemoveMode;
	std::string channelName = _commandArray[1];
	Channel &channel = _ircServ.getChannel(channelName);
	std::string modes = _commandArray[2];
	std::map<std::string, Client> &users = channel.getUsers();
	if (argIndex >= _commandArray.size())
		return (void)ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "MODE"));
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
		return (void)ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "MODE no user"));
}
