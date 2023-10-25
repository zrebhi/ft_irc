/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:03:43 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/24 21:33:54 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Command::Command(const std::vector <std::string> &commandArray, Client &client, Server &ircServ) :
_commandArray(commandArray), _client(client) , _ircServ(ircServ) {}

Command::~Command() {}

bool Command::channelExists(std::string channelName) {
	if (channelName[0] == '#')
		channelName = channelName.substr(1);

	std::map<std::string, Channel> channelList = this->_ircServ.getChannelList();
	std::map<std::string, Channel>::iterator it = channelList.find(channelName);
	if (it == channelList.end()) {
		if (this->_commandArray[0] != "JOIN")
			ft_send(this->_client, ERR_NOSUCHCHANNEL(this->_client, channelName));
		return false;
	}
	else
		return true;
}

bool Command::IsChannelMember(std::string userNickname, std::string channelName) {
	if (channelName[0] == '#')
		channelName = channelName.substr(1);
	std::map<std::string, Channel> channelList = this->_ircServ.getChannelList();

	std::map<std::string, Client> channelUsers = channelList[channelName].getUsers();
	std::map<std::string, Client>::iterator it = channelUsers.find(userNickname);
	if (it == channelUsers.end()) {
		std::string reply = ":IRC 442 " + this->_client.getNickname() + " " + channelName + " :You're not on that channel";
		ft_send(this->_client, reply);
		return false;
	}
	else
		return true;
}

bool Command::registerRequest() {
	if (this->_commandArray[0] == "PASS")
		pass();
	else if (this->_commandArray[0] == "NICK")
		nick();
	else if (this->_commandArray[0] == "USER")
		user();
	else if (this->_client.isRegistered())
		return true;
	else if (this->_commandArray[0] != "CAP") {
		ft_send(_client, ERR_NOTREGISTERED(this->_client));
		ft_send(_client, "You need to enter correct NICK, USER and PASS content");
	}
	return false;
}

bool Command::validServerPassword() {
	if (!this->_ircServ.isProtected() || this->_ircServ.getServerPassword() == this->_client.getPassword())
		return true;
	ft_send(this->_client, ERR_PASSWDMISMATCH);
	return false;
}

std::map<int, Client>::iterator Command::findClientOnServer(const std::string &nickname) {
	std::map<int, Client>::iterator it;
    for (it = _ircServ.getClientList().begin(); it != _ircServ.getClientList().end(); ++it)
	{
        if (it->second.getNickname() == nickname)
            return it;
    }
    return _ircServ.getClientList().end();
}

bool Command::validChannelName(std::string channelName) {
	if (channelName[0] != '#' || channelName.size() < 2)
		return false;
	std::string allowedSpecialChars = "-[]`^{}\'\"";
	for (size_t i = 1; i < channelName.size(); i++) {
		if (!std::isalnum(channelName[i]) && allowedSpecialChars.find(channelName[i]) == allowedSpecialChars.npos)
			return false;
	}
	return true;
}

std::string Command::formatChannelName(std::string channelName) {
	channelName = channelName.substr(1);
	for (size_t i = 0; i < channelName.size(); i++) {
		channelName[i] = std::tolower(channelName[i]);
	}
	return channelName;
}
