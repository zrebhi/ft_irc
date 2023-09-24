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
#include <arpa/inet.h>
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
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

void Server::ClientInput(size_t i)
{
	// to clear
	static int endCount;

	if (_clientList[i]->getPseudo() == "")
		setNickname(i);
	else
	{
		endCount++;
		std::cout << "Client Created and handle message" << std::endl;
		char buffer[1000];
		size_t bytesRead = recv(_fdList[i].fd, buffer, sizeof(buffer), 0);
		// parsing et gerer l'input
		if (endCount > 4)
		{
			send(_fdList[i].fd, "stop", 4, 0);
			_serverIsUp = false;
		}
		else if (bytesRead)
			std::cout << buffer << std::endl;
	}
}

bool Server::nickIsExisting(char *buffer)
{
	std::map<int, Client *>::iterator it = _clientList.begin();
	while (it != _clientList.end())
	{
		if (it->second->getPseudo() == buffer)
			return true;
		it++;
	}
	return false;
}

void Server::setNickname(size_t i)
{
	char buffer[1000];
	std::string nick = "Enter your nickname: ";
	std::string wrongNickSize = "Size of nickname is less than five.\n";
	std::string nickExisting = "Nickname is already attributed.\n";
	send(_fdList[i].fd, nick.c_str(), nick.size(), 0);
	int bytesRead = recv(_fdList[i].fd, buffer, sizeof(buffer), 0);
	if (bytesRead < 5)
		send(_fdList[i].fd, wrongNickSize.c_str(), wrongNickSize.size(), 0);
	else if (nickIsExisting(buffer))
		send(_fdList[i].fd, nickExisting.c_str(), nickExisting.size(), 0);
	else
	{
		buffer[bytesRead] = '\0';
		std::string pseudo(buffer);
		std::cout << buffer << ": is the new pseudo: " << pseudo << std::endl;
		if (!_clientList[i])
			std::cout << "no client in this list" << std::endl;
		_clientList[i]->setPseudo(pseudo);
		std::cout << _clientList[i]->getPseudo() << ": check new pseudo" << std::endl;
	}
}

void Server::serverSetup()
{
	createSocket();
	bindSocket();
	setSocketToListen();
	acceptConnection(0);
	_serverIsUp = true;
	while (_serverIsUp)
	{
		int pollSize = poll(_fdList, _usersOnline, -1);
		if (pollSize == -1)
		{
			std::cerr << "|";
			sleep(2);
			continue;
		}
		std::cerr << std::endl;
		for (size_t i = 0; i < _usersOnline; i++)
		{
			if (_fdList[i].revents & POLLIN)
			{
				if (_fdList[i].fd == _serverSocket) // si c'est une nouvelle co
					acceptConnection(i);
				else
					ClientInput(i);
			}
		}
	}
	close(_serverSocket);
	std::map<int, Client *>::iterator it = _clientList.begin();
	while (it != _clientList.end())
	{
		close(it->first);
		delete (it->second);
		it++;
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

void Server::acceptConnection(size_t i)
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
	_fdList[i].fd = newSocket;
	_fdList[i].events = POLLIN;
	_clientList[i] = new Client("", newSocket);
	std::cout << "connection accepted from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;
	// printf("Connection accepted from %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));
	setNickname(i);
}
