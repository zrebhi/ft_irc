#include "Command.hpp"
#include <map>

void Command::invite()
{
	if (_commandArray.size() < 3 || _commandArray[1].empty() || _commandArray[2].empty())
		ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, _commandArray[0]));

	const std::string &targetName = _commandArray[1];
	std::string &channelName = _commandArray[2];
	std::map<int, Client>::iterator targetClientIt = findClientOnServer(targetName);

	if (channelName.empty() || channelName.at(0) != '#')
		return (void)ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	channelName = channelName.substr(1);
	if (!channelExists(channelName))
		return (void)ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	Channel &channel = _ircServ.getChannel(channelName);
	if (channel.isChannelLocked() && !channel.isOperator(_client.getNickname()))
		return (void)ft_send(this->_client, ERR_CHANOPRIVSNEEDED(channelName, _client));
	if (targetClientIt == _ircServ.getClientList().end())
		return (void)ft_send(this->_client, ERR_NOSUCHNICKCHAN(_client, channelName, targetName, "INVITE"));
	if (channel.isUserInChannel(targetName))
		return (void)ft_send(this->_client, ERR_USERONCHANNEL(targetName, channelName));
	channel.setInvitedList(targetName, ADD);
	ft_send(this->_client, RPL_INVITING(channelName, targetName));
	ft_send(targetClientIt->second, RPL_INVITE_RCV(_client, channelName, targetName));
}
