#include "Command.hpp"
#include <cstddef>
#include <string>
#include <vector>

void Command::part()
{
	if (_commandArray.size() < 2 || _commandArray[1].empty())
		return ft_send(_client, ERR_NEEDMOREPARAMS(_client, _commandArray[0]));
	std::vector<std::string> channelNames = ft_split(_commandArray[1], ',');
	std::string partMessage;
	for(size_t i = 2; i < _commandArray.size(); i++)
	{
		if (i != 2)
			partMessage.push_back(' ');
		partMessage.append(_commandArray[i]);
	}
	if (partMessage.empty())
		partMessage = ":no reason given.";
	partMessage.push_back('\n');
	for (std::vector<std::string>::iterator it = channelNames.begin(); it != channelNames.end(); it++)
	{
		std::string channelName = *it;
		if (!validChannelName(channelName))
		{
			ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
			continue;
		}
		channelName = formatChannelName(channelName);
		if (!channelExists(channelName))
			continue;
		Channel &channel = _ircServ.getChannel(channelName);
		if (!channel.isUserInChannel(_client.getNickname()))
		{
			ft_send(this->_client, ERR_NOTONCHANNEL(channelName));
			continue;
		}
		std::string reply = ":" + _client.getNickname() + "!" + _client.getUsername() + \
			"@" + "IRC PART #" + channelName + " " + partMessage;
		channel.deleteClient(_client.getNickname(), reply);
		if (channel.getUsers().empty())
			_ircServ.getChannelList().erase(channelName);
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
	while (it != _ircServ.getChannelList().end()) {
		it->second.deleteClient(_client.getNickname(), reply);

		if (it->second.getUsers().empty()) {
			std::map<std::string, Channel>::iterator eraseIt = it;
			++it;  // increment before erasing
			_ircServ.getChannelList().erase(eraseIt);
		} else
			++it;  // increment here
	}
	this->_ircServ.removeClientFromServer(_client);
}
