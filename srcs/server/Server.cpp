/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 19:25:37 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/11 18:34:18 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "../commands/Command.hpp"

void Server::listenToNewEvents() {
	struct pollfd *fds = new struct pollfd[this->_clients.size() + 1];
	pollSetup(fds);

	int ret = poll(fds, this->_clients.size() + 1, 10);  // Wait indefinitely for an event
	if (ret == -1) {
		perror("Failed to poll file descriptors");
		exit(1);
	}

	manageEvents(fds);
	delete[] fds;
}

void Server::pollSetup(struct pollfd *fds) {
	fds[0].fd = this->_serverSocket;
	fds[0].events = POLLIN;

	if (this->_clients.empty())
		return;

	std::vector<Client>::iterator it = this->_clients.begin();
	for (size_t i = 1; it != this->_clients.end(); ++it, ++i) {
		fds[i].fd = it->getSocket();  // Get the socket from the map key
		fds[i].events = POLLIN;
	}
}

void Server::manageEvents(struct pollfd *fds) {
	if (fds[0].revents & POLLIN) {
		this->acceptConnection();
		return;
	}

	std::vector<Client>::iterator it = this->_clients.begin();
	for (size_t i = 1; it != this->_clients.end(); ++it, ++i) {
		if (this->_clients.empty())
			return;
		if (fds[i].revents & POLLIN) {
			char buffer[1024];
			size_t bytesRead = recv(it->getSocket(), buffer, sizeof(buffer) - 1, 0);
			if (bytesRead <= 0) {
				close(it->getSocket());
				it = this->_clients.erase(it);
			} else {
				buffer[bytesRead] = '\0';  // Null-terminate the string
				std::string bufferString(buffer);
				std::cout << "<- " << bufferString << std::endl;
				commandHandler(bufferString, *it);
			}
		}
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
	this->_clients.push_back(Client(newSocket));
	std::cout << "Connection accepted from " << inet_ntoa(clientAddress.sin_addr) << ":"
			  << ntohs(clientAddress.sin_port) << std::endl;
}

void Server::commandHandler(std::string bufferString, Client &client) {
	std::vector <std::string> commands = ft_split(bufferString, '\n');

	for (size_t i = 0; i < commands.size(); i++) {
		std::vector <std::string> commandArray = ft_split(commands[i], ' ');
		Command cmd = Command(commandArray, client, *this);
		if (commandArray[0] == "NICK")
			cmd.nick();
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
	}
}
