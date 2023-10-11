/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 20:59:05 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/11 00:45:34 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::map<std::string, Channel> Server::getChannelList() const {
	return this->_channels;
}

// Must be used with channelExists;
Channel Server::getChannel(std::string channelName) {
	if (channelName[0] == '#')
		channelName = channelName.substr(1);
	std::map<std::string, Channel>::iterator	it = this->_channels.find(channelName);
	return it->second;
}

std::vector<Client>	Server::getClientList() const {
	return this->_clients;
}

int Server::getServerSocket() {
	return this->_serverSocket;
}

void Server::serverShutdown() {
	this->_serverUp = false;
}

bool Server::serverRunning() {
	if (this->_serverUp)
		return true;
	else
		return false;
}
