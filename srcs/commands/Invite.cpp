#include "Command.hpp"
#include <map>

void Command::invite()
{
	if (_commandArray.size() < 3 || _commandArray[1].empty() || _commandArray[2].empty())
		return ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, _commandArray[0]));

	const std::string &targetName = _commandArray[1];

	std::string &channelName = _commandArray[2];
	if (!validChannelName(channelName))
		return ft_send(this->_client, ERR_INVALIDCHANNEL(_client, channelName));
	channelName = formatChannelName(channelName);
	if (!channelExists(channelName))
		return;

	std::map<int, Client>::iterator targetClientIt = findClientOnServer(targetName);

	Channel &channel = _ircServ.getChannel(channelName);
	if (!channel.isUserInChannel(_client.getNickname()))
		return ft_send(_client, ERR_NOTONCHANNEL(channelName));
	if (channel.isInviteOnly() && !channel.isOperator(_client.getNickname()))
		return ft_send(this->_client, ERR_CHANOPRIVSNEEDED(channelName, _client));
	if (targetClientIt == _ircServ.getClientList().end())
		return ft_send(this->_client, ERR_NOSUCHNICKCHAN(_client, channelName, targetName, "INVITE"));
	if (channel.isUserInChannel(targetName))
		return ft_send(this->_client, ERR_USERONCHANNEL(targetName, channelName));
	channel.setInvitedList(targetName, ADD);
	ft_send(targetClientIt->second, RPL_INVITE_RCV(_client, channelName, targetName));
	channel.serverMessageToChannel(RPL_INVITE_RCV(_client, channelName, targetName));
}
