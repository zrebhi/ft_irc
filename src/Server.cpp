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
#include <cstddef>
#include <iostream>
#include <string>

Server::Server()
	: portNumber(8080), _serverIsUp(false)
{
	Server::serverSetup();
}

// Traite la commande JOIN a  partir d'un client
void Server::joinHandler(t_cmd *input, Client *client)
{
	std::string addFail = "Error: add user failed";
	std::string addSuccess = "Success: add user ";
	std::vector<Channels>::iterator it = _channels.begin();
	if (input->params.size() > 1)
		return;
	while (it != _channels.end())
	{
		if (it->getChanName().compare(input->params[0]))
		{
			if (!it->addUser(client))
				send(client->getSocket(), addFail.c_str(), addFail.size(), 0);
			else
			{
				addSuccess.append(client->getNickname());
				addSuccess.append(" to ");
				addSuccess.append(it->getChanName());
				send(client->getSocket(), addSuccess.c_str(), addSuccess.size(), 0);
			}
			return;
		}
		it++;
	}
	_channels.push_back(Channels(input->params[0]));
	addSuccess.append(client->getNickname());
	addSuccess.append(" to ");
	addSuccess.append(_channels[0].getChanName());
	send(client->getSocket(), addSuccess.c_str(), addSuccess.size(), 0);
	_channels[0].addUser(client);
}

// Traite la commande NICK
void Server::nickHandler(t_cmd *input, Client *client)
{
	std::string argsNb = "Error: too many arguments";
	std::string nickLen = "Error: wrong nick length";
	std::string newNickname = "New pseudo: ";
	if (input->params.size() != 1)
	{
		std::cout << RED << "\t" << argsNb << RESET << std::endl;
		send(client->getSocket(), argsNb.c_str(), argsNb.size(), 0);
	}
	else if (input->params[0].size() < 3 || input->params[0].size() > 12)
	{
		std::cout << RED << "\t" << nickLen << RESET << std::endl;
		send(client->getSocket(), nickLen.c_str(), nickLen.size(), 0);
	}
	else
	{
		client->setNickname(input->params[0]);
		std::cout << RED << "\tnew pseudo: " << client->getNickname() << RESET << std::endl;
		newNickname.append(client->getNickname());
		send(client->getSocket(), newNickname.c_str(), nickLen.size(), 0);
	}
}

// Traite les messages prinves PRIVMSG (pas encore les messages du Channel)
void Server::messageHandler(t_cmd *input, Client *client)
{
	std::map<int, Client *>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (it->second->getNickname().compare(input->params[0]))
		{
			send(it->second->getSocket(), input->params[1].c_str(), input->params[1].size(), 0);
			return;
		}
		it++;
	}
	std::string notFound = "client not found";
	send(client->getSocket(), notFound.c_str(), notFound.size(), 0);
}

// Commande USER  pour mettre a jour le profil du client
void Server::userHandler(t_cmd *input, Client *client)
{
	std::string argsNb = "Error: too many arguments";
	if (input->params.size() < 5)
		send(client->getSocket(), argsNb.c_str(), argsNb.size(), 0);
	else
	{
		client->setUsername(input->params[0]);
		std::string realname;
		for (size_t i = 4; i < input->params.size(); i++)
		{
			if (i != 4)
				realname.push_back(' ');
			realname.append(input->params[i]);
		}
		client->setRealname(realname);
	}
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
	close(_epfd);
	for (size_t i = 0; i < _clients.size(); i++)
		if (_clients[i])
			close(_clients[i]->getSocket());
}

// recois la commande du client -> parseInput renvoi un strcuture t_cmd
// avec la commande + les arguments (un peu comme minishell);
// en fonction de la commande recue, il renvoi vers la bonne fonction
void Server::handleClient(Client *client)
{
	char buffer[1000];
	memset(buffer, 0, sizeof(buffer));
	recv(client->getSocket(), buffer, 1000, 0);
	std::cout << RED << "\tServer recv: " << buffer << RESET << std::endl;
	if (strncmp(buffer, "stop", 4) == 0)
	{
		_serverIsUp = false;
		return;
	}
	t_cmd *parsedInput = parseInput(buffer);
	if (parsedInput->cmdType.compare("NICK") == 0)
		nickHandler(parsedInput, client);
	else if (parsedInput->cmdType.compare("PRIVMSG") == 0)
		messageHandler(parsedInput, client);
	else if (parsedInput->cmdType.compare("USER") == 0)
		userHandler(parsedInput, client);
	else if (parsedInput->cmdType.compare("JOIN") == 0)
		joinHandler(parsedInput, client);
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
	std::cout << RED << "\tEnter acceptConnection" << RESET << std::endl;
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
	printf("\t%sConnection accepted from %s:%d %s\n", RED, inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port), RESET);
	struct epoll_event event;
	event.events = EPOLLIN;
	event.data.fd = newSocket;
	if (epoll_ctl(_epfd, EPOLL_CTL_ADD, newSocket, &event))
	{
		perror("failed add client - acceptConnection");
		exit(1);
	}
	send(newSocket, "Welcome!", 8, 0);
}
