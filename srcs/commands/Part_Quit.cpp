#include "Command.hpp"
#include <cstddef>
#include <string>
#include <vector>

void Command::part()
{
	if (_commandArray.size() < 2)
		return (void)ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "PART"));
	std::vector<std::string> channelNames = ft_split(_commandArray[1], ',');
	std::string partMessage;
	for(size_t i = 2; i < _commandArray.size(); i++)
	{
		if (i != 2)
			partMessage.push_back(' ');
		partMessage.append(_commandArray[i]);
	}
	if (partMessage.empty())
		partMessage = "no reason given.";
	if (partMessage.at(0) == ':')
		partMessage = partMessage.substr(1);
	partMessage.push_back('\n');
	for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); it++)
	{
		std::string channelName = *it;
		if (channelName.empty() || channelName.at(0) != '#')
			return (void)ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
		channelName = channelName.substr(1);
		if (!channelExists(channelName))
			return (void)ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
		Channel &channel = _ircServ.getChannel(channelName);
		if (channel.getUsers().find(_client.getNickname()) == channel.getUsers().end())
			return (void)ft_send(this->_client, ERR_NOTONCHANNEL(channelName));
		std::string reply = ":" + _client.getNickname() + "!" + _client.getUsername() + \
			"@" + "IRC PART #" + channelName + " :" + partMessage;
		channel.deleteClient(_client.getNickname(), reply);
		ft_send(_client, reply);
	}
}

void Command::quit()
{
	std::map<std::string, Channel>::iterator it = _ircServ.getChannelList().begin();
	std::string reply = ":" + _client.getNickname() + "!" + \
		_client.getUsername() + "@" + "IRC QUIT ";

	if (_commandArray.size() >= 2)
	{
		for (size_t i = 1; i < _commandArray.size(); i++)
		{
			reply.append(_commandArray[1]);
			if (i != _commandArray.size() - 1)
				reply.push_back(' ');
		}
	}
	else
		reply.append(":leaving the channel.");
	reply.push_back('\n');
	for (;it != _ircServ.getChannelList().end(); it++)
		it->second.deleteClient(_client.getNickname(), reply);
}
