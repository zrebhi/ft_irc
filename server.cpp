/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 19:25:37 by zrebhi            #+#    #+#             */
/*   Updated: 2023/09/21 19:58:16 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() {
	Server::serverSetup();
}

Server::~Server() {}

void	Server::serverSetup() {
	createSocket();
	bindSocket();
	setSocketToListen();
	while (1) {
		acceptConnection();
	}
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
	printf("Server socket created successfully\n");
}

void	Server::acceptConnection() {
	struct sockaddr_in	clientAddress;
	int	newSocket;

	socklen_t	clientAddressLength = sizeof(clientAddress);
	newSocket = accept(this->_serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
	if (newSocket == -1) {
		perror("Failed to accept incoming connection");
		close(this->_serverSocket);
		exit(1);
	}
	this->_clientSockets.push_back(newSocket);
	printf("Connection accepted from %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
}
