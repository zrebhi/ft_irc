/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSetup.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 23:16:50 by zrebhi            #+#    #+#             */
/*   Updated: 2023/09/28 23:16:50 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {
	Server::serverSetup();
}

Server::~Server() {}

Server::Server(const Server &src) :
_serverSocket(src._serverSocket), _clientSockets(src._clientSockets),
_channels(src._channels), _serverAddress(src._serverAddress) {}

void	Server::serverSetup() {
	close(this->_serverSocket);
	createSocket();
	bindSocket();
	setSocketToListen();
}

void	Server::createSocket() {
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (this->_serverSocket == -1) {
		perror("Failed to create socket");
		exit(1);
	}
}

void	Server::bindSocket() {
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	this->_serverAddress.sin_port = htons(portNumber);

	if (bind(this->_serverSocket, (struct sockaddr *) &this->_serverAddress, \
	sizeof(this->_serverAddress)) == -1) {
		perror("Failed to bind socket to address");
		close(this->_serverSocket);
		exit(1);
	}
}

void	Server::setSocketToListen () {
	int backlog = 5;  // maximum number of pending connections

	if (listen(this->_serverSocket, backlog) == -1) {
		perror("Failed to listen for connections");
		close(this->_serverSocket);
		exit(1);
	}
	std::cout << "Server socket created successfully" << std::endl;
}