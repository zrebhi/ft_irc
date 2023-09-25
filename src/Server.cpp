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
#include <iostream>
#include <map>

Server::Server()
	: portNumber(8080), _serverIsUp(false)
{
	_serverEvent.events = EPOLLIN;
	_serverEvent.data.fd = _serverSocket;
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

	_serverIsUp = true;
	std::vector<struct epoll_event> events(10);
	while (_serverIsUp)
	{
		std::cout << "1" << std::endl;
		int nbEvents = epoll_wait(_epfd, events.data(), static_cast<int>(events.size()), -1);
		if (nbEvents == -1)
		{
			std::cerr << "epoll wait failed" << std::endl;
			break;
		}
		for (int i = 0; i < nbEvents; i++)
		{
			int currentFd = events[i].data.fd;
			if (currentFd == _serverSocket) // si c'est une nouvelle co
				acceptConnection(_epfd);
			else
				handleClient(_clientSockets[currentFd]);
		}
	}
	close(_serverSocket);
	close(_epfd);
	for (size_t i = 0; i < _clientList.size(); i++)
		close(_clientList[i].getSocket());
}

void Server::handleClient(Client *client)
{
	// if (client->getPseudo().empty())
	// 	send(client->getSocket(), "Please enter a valid NICK", 8, 0);
	// else
	// {
	_serverIsUp = false;
	char buffer[1000];
	memset(buffer, 0, sizeof(buffer));
	std::cout << "message from client " << client->getSocket() << std::endl;
	recv(client->getSocket(), buffer, 1000, 0);
	std::cout << buffer << std::endl;
	// }
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
	_epfd = epoll_create1(0);
	if (_epfd == -1)
	{
		perror("failed to create epoll");
		exit(1);
	}
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, _serverSocket, &_serverEvent) == -1)
	{
		perror("failed to create epoll");
		close(_epfd);
		exit(1);
	}
}

void Server::acceptConnection(int _epfd)
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
	_clientSockets[newSocket] = new Client("", newSocket);
	printf("Connection accepted from %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
	struct epoll_event newEvent;
	newEvent.events = EPOLLIN;
	newEvent.data.fd = newSocket;
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, newSocket, &newEvent))
	{
		perror("failed add client - acceptConnection");
		exit(1);
	}
	send(newSocket, "Welcome!", 8, 0);
}

void Server::parseString(char *receivedMessage)
{
	std::vector<char *> ptrArray;
	char *tempPtr = receivedMessage;
	if (tempPtr && *tempPtr)
		ptrArray.push_back(tempPtr);
	while (tempPtr && *tempPtr)
	{
		tempPtr++;
	}
}
