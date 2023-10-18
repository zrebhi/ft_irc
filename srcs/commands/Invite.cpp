#include "Command.hpp"
#include <map>

void Command::invite()
{
	std::map<int, Client>::const_iterator allClientsIt = _ircServ.getClientList().begin();
	if (_commandArray.size() < 3)
		return ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, "INVITE"));
	const std::string &ClientName = _commandArray[1];
	std::string &channelName = _commandArray[2];
	if (channelName.empty() || channelName.at(0) != '#')
		return ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	std::cout << "one" << std::endl;
	channelName = channelName.substr(1);
	if (!channelExists(channelName))
		return ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
	std::cout << "two" << std::endl;
	Channel &channel = _ircServ.getChannel(channelName);
	if (channel.isChannelLocked() && !channel.isOperator(ClientName))
		return ft_send(this->_client, ERR_CHANOPRIVSNEEDED(channelName));
	std::cout << "three" << std::endl;
	if (allClientsIt == _ircServ.getClientList().end())
		return ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, "no user")); //change message to no such user
	std::cout << "four" << std::endl;
	if (channel.getUsers().find(ClientName) != channel.getUsers().end())
		return ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, "is logged")); //change message to already log
	std::cout << "five" << std::endl;
	std::string reply = ":IRC 341 #" + channelName + " " + ClientName;
	channel.setInvitedList(ClientName, ADD);
	ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
}
