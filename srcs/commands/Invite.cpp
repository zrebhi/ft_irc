#include "Command.hpp"
#include <map>

void Command::invite()
{
	if (_commandArray.size() < 2 || _commandArray[1].empty())
		ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, _commandArray[0]));

	const std::string &targetName = _commandArray[1];
	std::string &channelName = _commandArray[2];
	std::map<int, Client>::const_iterator targetClientIt = findClientOnServer(targetName);

	if (channelName.empty() || channelName.at(0) != '#')
		return (void)ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	channelName = channelName.substr(1);
	if (!channelExists(channelName))
		return (void)ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	Channel &channel = _ircServ.getChannel(channelName);
	if (channel.isChannelLocked() && !channel.isOperator(targetName))
		return (void)ft_send(this->_client, ERR_CHANOPRIVSNEEDED(channelName));
	if (targetClientIt == _ircServ.getClientList().end())
		return (void)ft_send(this->_client, ERR_NOSUCHNICK(targetName));
	if (channel.isUserInChannel(targetName))
		return (void)ft_send(this->_client, ERR_USERONCHANNEL(channelName, targetName));
	channel.setInvitedList(targetName, ADD);
	ft_send(this->_client, RPL_INVITING(channelName, targetName));
	ft_send(targetClientIt->second, RPL_INVITING(channelName, targetName));
}
