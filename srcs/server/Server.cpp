/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 19:25:37 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/14 01:40:23 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstring>
#include <iostream>
#include <map>
#include <string>

void Server::listenToNewEvents() {
	struct epoll_event events[10];
	int numEvents = epoll_wait(this->_epollFd, events, 10, -1);

	for (int i = 0; i < numEvents; ++i) {
		if (events[i].data.fd == this->_serverSocket)
			acceptConnection();
		else {
			std::map<int, Client>::iterator clientIt = _clients.find(events[i].data.fd);
			if (clientIt != _clients.end())
				manageClientEvents(clientIt->second);
		}
	}
}

void Server::manageClientEvents(Client &client) {
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	ssize_t bytesRead = recv(client.getSocket(), buffer, sizeof(buffer) - 1, 0);
	if (bytesRead == 0 || (bytesRead == -1 && errno == ECONNRESET)) {
		close(client.getSocket());
		if (epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, client.getSocket(), NULL))
			std::cout << "failed to remove epoll" << std::endl;
		std::string reply = "crash";
		getClientList().erase(client.getSocket());
		std::map<std::string, Channel>::iterator it = _channels.begin();
		while (it != _channels.end())
			it->second.deleteClient(client.getNickname(), reply);
	}
	else {
		std::string bufferString(buffer);
		std::cout << "<- " << bufferString << std::endl;
		commandHandler(bufferString, client);
	}
}

void Server::acceptConnection() {
	struct sockaddr_in clientAddress;
	int newSocket;
	socklen_t clientAddressLength = sizeof(clientAddress);
	newSocket = accept(this->_serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
	if (newSocket == -1) {
		perror("Failed to accept incoming connection");
		close(this->_serverSocket);
		exit(1);
	}
	this->_clients.insert(std::make_pair(newSocket, Client(newSocket)));
	addSocketToEpoll(newSocket);
	std::cout << "Connection accepted from " << inet_ntoa(clientAddress.sin_addr) << ":"
			  << ntohs(clientAddress.sin_port) << std::endl;
}

void Server::commandHandler(std::string bufferString, Client &client) {
	std::vector <std::string> commands = ft_split(bufferString, '\n');
	std::vector <std::string> commandArray;

	for (size_t i = 0; i < commands.size(); i++) {
		commandArray = ircCommandSplitter(commands[i]);
		Command cmd = Command(commandArray, client, *this);
		if (!cmd.registerRequest() && !client.isRegistered())
			continue;
		if (_commandMap.find(commandArray[0]) != _commandMap.end()) {
			CommandFunction function = _commandMap[commandArray[0]];
			(cmd.*function)();
		}
	}
}

bool Server::passwordIsValid(const std::string &password)
{
	return (_password == password);
}
