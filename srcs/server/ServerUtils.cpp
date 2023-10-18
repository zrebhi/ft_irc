/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 20:59:05 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/17 20:12:37 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::addSocketToEpoll(int socket) {
	struct epoll_event event = {};
	event.events = EPOLLIN;
	event.data.fd = socket;
	if (epoll_ctl(this->_epollFd, EPOLL_CTL_ADD, socket, &event) == -1) {
		perror("Failed to add server socket to epoll");
		close(this->_epollFd);
		return;
	}
}

void Server::addChannelToServer(Channel channel) {
	this->_channels[channel.getName()] = channel;
}

std::string Server::getServerPassword() {
	return _password;
}

std::map<std::string, Channel>& Server::getChannelList() {
	return this->_channels;
}

// Must be used with channelExists;
Channel& Server::getChannel(std::string channelName) {
	if (channelName[0] == '#')
		channelName = channelName.substr(1);
	std::map<std::string, Channel>::iterator	it = this->_channels.find(channelName);
	return it->second;
}

const std::map<int, Client>	&Server::getClientList() const {
	return this->_clients;
}

int Server::getServerSocket() {
	return this->_serverSocket;
}

int Server::getEpollFd() {
	return this->_epollFd;
}

void Server::serverShutdown() {
	this->_serverUp = false;
}

bool Server::serverRunning() {
	return this->_serverUp;
}

bool Server::isProtected() {
	return (_password.empty() == false);
}
