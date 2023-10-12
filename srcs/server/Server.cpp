/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 19:25:37 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/11 21:14:00 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
	if (bytesRead == 0 || (bytesRead == -1 && errno == ECONNRESET)) {
		close(client.getSocket());
		epoll_ctl(this->_epollFd, EPOLL_CTL_DEL, client.getSocket(), NULL);
		this->_clients.erase(client.getSocket());
	}
	else {
		buffer[bytesRead] = '\0';  // Null-terminate the string
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

	for (size_t i = 0; i < commands.size(); i++) {
		std::vector <std::string> commandArray = ft_split(commands[i], ' ');
		Command cmd = Command(commandArray, client, *this);
		if (commandArray[0] == "PASS")
			cmd.pass();
		if (client.getRegistered() == false) // not logged
			continue;
		if (commandArray[0] == "NICK")
			cmd.nick();
		if (commandArray[0] == "MODE")
			cmd.mode(this->_channels);
		if (commandArray[0] == "USER")
			cmd.user();
		if (commandArray[0] == "JOIN")
			cmd.join(this->_channels);
		if (commandArray[0]== "PRIVMSG")
			cmd.privmsg();
		if (commandArray[0] == "WHO")
			cmd.who();
		if (commandArray[0] == "STOP")
			cmd.shutdown();
		//PART pour quitter un channel
	}
}

bool Server::isProtected()
{
	return (_password.empty() == false);
}

bool Server::passwordIsValid(std::string &password)
{
	return (_password == password);
}
