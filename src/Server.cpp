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
#include <cstring>
#include <sys/epoll.h>
#define MAX_CHARS 256
#define TOO_LONG "WARNING: Message are limited to 256 characters, remaining will be ignored.\n"

#include "../inc/Message.hpp"
#include "../inc/Server.hpp"

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
		std::cerr << "wrong port value" << RESET << std::endl;
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
	epollSetup();

	_serverIsUp = true;
	std::vector<struct epoll_event> events(2);
	while (_serverIsUp)
	{
		int nbEvents = epoll_wait(_epfd, events.data(), static_cast<int>(events.size()), -1);
		if (nbEvents == -1)
		{
			std::cerr << "epoll wait failed" << RESET << std::endl;
			break;
		}
		for (int i = 0; i < nbEvents; i++)
		{
			int currentFd = events[i].data.fd;
			if (currentFd == _serverSocket) // si c'est une nouvelle co
				acceptConnection(_epfd);
			else
				handleClient(_clients[currentFd]);
		}
	}
	close(_serverSocket);
	// close(_epfd);
	// for (size_t i = 0; i < _clients.size(); i++)
	// 	if (_clients[i])
	// 		close(_clients[i]->getSocket());
}

// recois la commande du client -> parseInput renvoi un strcuture t_cmd
// avec la commande + les arguments (un peu comme minishell);
// en fonction de la commande recue, il renvoi vers la bonne fonction
void Server::removeClient(Client *client)
{
	// QUIT => envoye un message a tous les canaux ou le client est connecte (via Server a removeClient ?)
	struct epoll_event event;
	event.events = EPOLLIN;
	epoll_ctl(_epfd, EPOLL_CTL_DEL, client->getSocket(), &event);
	close(client->getSocket());
	_clients.erase(client->getSocket());
}

void Server::handleClient(Client *client)
{
	char buffer[MAX_CHARS];
	memset(buffer, 0, sizeof(buffer));
	int bytesReceived = recv(client->getSocket(), buffer, MAX_CHARS - 1, 0);
	if (bytesReceived <= 0)
		removeClient(client);
	if (bytesReceived > MAX_CHARS)
		send(client->getSocket(), TOO_LONG, sizeof(TOO_LONG), 0);
	std::cout << GREEN << "\tServer recv: " << buffer << RESET << std::endl;
	std::istringstream bufferStream(buffer);
	std::string bufferLine;
	while (std::getline(bufferStream, bufferLine))
	{
		if (!strncmp(bufferLine.c_str(), "QUIT", 4))
			_serverIsUp = false;
		Message currentCmd = Message(_channels, _clients);
		if (currentCmd.handleInput(client, bufferLine) == true)
		{
			removeClient(client);
			break;
		}
	}
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

void Server::epollSetup()
{
	_epfd = epoll_create1(0);
	if (_epfd == -1)
	{
		perror("failed to create epoll");
		exit(1);
	}
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = _serverSocket;
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, _serverSocket, &event) == -1)
	{
		perror("failed to create epoll");
		close(_epfd);
		exit(1);
	}
}

void Server::acceptConnection(int _epfd)
{
	std::cout << GREEN << "\tEnter acceptConnection" << RESET << std::endl;
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
	_clients[newSocket] = new Client(newSocket);
	printf("\t%sConnection accepted from %s:%d %s\n", GREEN, inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port), RESET);
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = newSocket;
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, newSocket, &event))
	{
		perror("failed add client - acceptConnection");
		exit(1);
	}
	send(newSocket, "Welcome!\n", 9, 0);
}
