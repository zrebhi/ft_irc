/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 19:25:37 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/20 20:24:24 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cstdio>

void Server::listenToNewEvents() {
	struct epoll_event events[10];
	int numEvents = epoll_wait(this->_epollFd, events, 10, -1);

	for (int i = 0; i < numEvents; ++i) {
		if (events[i].data.fd == this->_serverSocket)
			acceptConnection();
		else {
			Client &client = this->_clients.find(events[i].data.fd)->second;
			manageClientEvents(client);
		}
	}
}

void Server::manageClientEvents(Client &client) {
	char buffer[1024];
	ssize_t bytesRead = recv(client.getSocket(), buffer, sizeof(buffer) - 1, 0);

	if (bytesRead <= 0)
		removeClientFromServer(client);
	else {
		buffer[bytesRead] = '\0';  // Null-terminate the string
		client.appendBuffer(buffer);
		if (client.getBuffer().at(client.getBuffer().size() - 1) == '\n') {
			std::cout << "<- " << client.getBuffer() << std::endl;
			commandHandler(client.getBuffer(), client);
			client.clearBuffer();
		}
	}
}

void Server::removeClientFromServer(Client &client) {
	struct epoll_event event;
	event.events = 0;
	event.data.fd = client.getSocket();
	std::cout << "disconnected client: " << client.getNickname() << std::endl;
	if (epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, client.getSocket(), &event) == -1)
		perror("failed to remove socket from epoll");
	close(client.getSocket());

	std::map<std::string, Channel>::iterator it = getChannelList().begin();
	for (;it != getChannelList().end(); it++)
	{
		std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + \
				"@" + "IRC QUIT :left the server.";
		it->second.deleteClient(client.getNickname(), reply);
	}
	getClientList().erase(client.getSocket());
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
		if (commandArray.empty())
			continue;
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
