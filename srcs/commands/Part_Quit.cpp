#include "Command.hpp"
#include <string>

void Command::part()
{
	if (_commandArray.size() < 2)
		return (void)ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "PART"));
	std::string channelName = _commandArray[1];
	if (channelName.empty() || channelName.at(0) != '#')
		return (void)ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	channelName = channelName.substr(1);
	if (!channelExists(channelName))
		return (void)ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	Channel &channel = _ircServ.getChannel(channelName);
	if (channel.getUsers().find(_client.getNickname()) == channel.getUsers().end())
		return (void)ft_send(this->_client, ERR_NOTONCHANNEL(channelName));
	std::string reply = ":" + _client.getNickname() + "!" + _client.getUsername() + \
		"@" + "IRC QUIT :";
	if (_commandArray.size() == 3)
		reply.append(_commandArray[2]);
	else
		reply.append("leaving.");
	reply.push_back('\n');
	channel.deleteClient(_client.getNickname(), reply);
}
