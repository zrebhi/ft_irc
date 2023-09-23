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

// TODO
// fonction exit pour free et fermer les fd
//

#include "../inc/Server.hpp"
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <sys/poll.h>
#include <sys/socket.h>

Server::Server()
	: portNumber(8080), _serverIsUp(false)
{
	Server::serverSetup();
}

Server::Server(unsigned int port)
	: _serverIsUp(false)
{
	if (portNumber < 9999)
	{
		this->portNumber = port;
		Server::serverSetup();
	}
	else
	{
		std::cerr << "wrong port value" << std::endl;
		exit(1);
	}
	// manage exit;
}

Server::~Server()
{
}

void Server::serverSetup()
{
	createSocket();
	bindSocket();
	setSocketToListen();
	acceptConnection();
	_serverIsUp = true;
	while (_serverIsUp)
	{
		int pollSize = poll(_fdList, _usersOnline, -1);
		if (pollSize == -1)
		{
			std::cerr << "poll failed" << std::endl;
			break;
		}
		for (size_t i = 0; i < _usersOnline; i++)
		{
			if (_fdList[i].revents & POLLIN)
			{
				if (_fdList[i].fd == _serverSocket) // si c'est une nouvelle co
					acceptConnection();
				else
				{
					// envoi a une autre f() pour gerer le message
					char buffer[1000];
					size_t bytesRead = read(_fdList[i].fd, buffer, sizeof(buffer));
					if (bytesRead)
						std::cout << buffer << std::endl;
				}
			}
		}
	}
	close(_serverSocket);
	for (size_t i = 0; i < _clientList.size(); i++)
		close(_clientList[i].getSocket());
}

void Server::createSocket()
{
	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (this->_serverSocket == -1)
	{
		perror("Failed to create socket");
		exit(1);
	}
}

void Server::bindSocket()
{
	this->_serverAddress.sin_family = AF_INET;
	this->_serverAddress.sin_addr.s_addr = INADDR_ANY;
	this->_serverAddress.sin_port = htons(portNumber);

	if (bind(this->_serverSocket, (struct sockaddr *)&this->_serverAddress, sizeof(this->_serverAddress)) == -1)
	{
		perror("Failed to bind socket to address");
		close(this->_serverSocket);
		exit(1);
	}
}

void Server::setSocketToListen()
{
	int backlog = 5; // maximum number of pending connections

	if (listen(this->_serverSocket, backlog) == -1)
	{
		perror("Failed to listen for connections");
		close(this->_serverSocket);
		exit(1);
	}
	printf("Server socket created successfully\n");
}

void Server::acceptConnection()
{
	struct sockaddr_in clientAddress;
	int newSocket;

	socklen_t clientAddressLength = sizeof(clientAddress);
	newSocket = accept(this->_serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
	if (newSocket == -1)
	{
		perror("Failed to accept incoming connection");
		close(this->_serverSocket);
		exit(1);
	}
	this->_clientSockets.push_back(newSocket);
	printf("Connection accepted from %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
	send(newSocket, "Welcome!", 8, 0);
}
