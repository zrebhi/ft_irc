/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 19:25:37 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/26 16:58:48 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <cctype>
#include <cstdio>
#include <ctime>
#include <string>
#include <vector>

void Server::listenToNewEvents() {
	struct epoll_event events[this->_clients.size() + 1];
	int numEvents = epoll_wait(this->_epollFd, events, this->_clients.size() + 1, -1);

	for (int i = 0; i < numEvents; ++i) {
		if (events[i].data.fd == this->_serverSocket)
			acceptConnection();
		else {
			Client &client = this->_clients.find(events[i].data.fd)->second;
			manageClientEvents(client);
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
	if (fcntl(newSocket, F_SETFL, O_NONBLOCK) == -1) {
		perror("Error setting socket as non-blocking");
		close(this->_serverSocket);
		exit(1);
	}
	this->_clients.insert(std::make_pair(newSocket, Client(newSocket)));
	if (!this->isProtected())
		this->_clients[newSocket].setRegistered(SERV_REGISTRATION);
	addSocketToEpoll(newSocket);
	std::cout << "Connection accepted from " << inet_ntoa(clientAddress.sin_addr) << ":"
			  << ntohs(clientAddress.sin_port) << std::endl;
}

void Server::manageClientEvents(Client &client) {
	char buffer[1024];

	ssize_t bytesRead = recv(client.getSocket(), buffer, sizeof(buffer) - 1, 0);
	if (bytesRead <= 0)
		removeClientFromServer(client);
	else {
		buffer[bytesRead] = '\0';
		if (!validBufferInput(bytesRead, buffer))
			return ft_send(client, ERR_TOOMANYMATCHES(client));
		client.appendBuffer(buffer);
		if (client.getBuffer().size() > 512 || client.getBuffer().size() < 1)
			return client.clearBuffer();
		if (client.getBuffer().at(client.getBuffer().size() - 1) == '\n') {
			std::cout << "<- \033[1;31m" << client.getBuffer() << "\033[0m" << std::endl;
			commandHandler(client.getBuffer(), client);
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
    if (it != getChannelList().end())
    {
        for (;it != getChannelList().end(); it++)
        {
            std::string reply = ":" + client.getNickname() + "!" + client.getUsername() + \
                    "@" + "IRC QUIT :Leaving";
            it->second.deleteClient(client.getNickname(), reply);
            if (it->second.getUsers().empty())
            {
                getChannelList().erase(it->first);
                it = getChannelList().begin();
            }
        }
    }
	getClientList().erase(client.getSocket());
}

bool	Server::validBufferInput(ssize_t bytesRead, std::string bufferInput) {
	if (bytesRead >= 512)
		return false;
	for (size_t i = 0; i < bufferInput.size(); i++) {
		if (!std::isprint(bufferInput[i]) && bufferInput[i] != '\r' && bufferInput[i] != '\n')
			return false;
	}
	return true;
}

void Server::commandHandler(std::string bufferString, Client &client) {
	std::vector <std::string> commands = ft_split(bufferString, '\n');
	std::vector <std::string> commandArray;

	for (size_t i = 0; i < commands.size(); i++) {
		commandArray = ircCommandSplitter(commands[i]);
		if (commandArray.empty() || strncmp(commands[i].c_str(), "CAP ", 4) == 0)
			continue;
		Command cmd = Command(commandArray, client, *this);
		if (commandArray[0] == "QUIT")
			return cmd.quit();
		if (!cmd.registerRequest() && client.isRegistered() != FULL_REGISTRATION)
			continue;
		if (_commandMap.find(commandArray[0]) != _commandMap.end()) {
			CommandFunction function = _commandMap[commandArray[0]];
			(cmd.*function)();
		}
	}
	client.clearBuffer();
}

bool Server::passwordIsValid(const std::string &password)
{
	return (_password == password);
}

bool Server::isFlooding(Client &client)
{
	std::time_t currentTime = std::time(NULL);
	// std::cout << client.getFloodCounter() << " - " << client.getFloodClock() << " vs " << currentTime << " ---> " << currentTime - client.getFloodClock() << std::endl;
	client.setFloodCounter(ADD);

	if (client.getFloodCounter() >= FLOOD_MAX_MSG \
		&& client.getFloodClock() + FLOOD_TIME < currentTime)
			return true;
	if (currentTime - client.getFloodClock() > FLOOD_TIME)
		client.setFloodCounter(REMOVE);
	return false;
}

bool Server::isUserBanned(const std::string& str) {
	std::vector<std::string>::const_iterator it = _bannedUsers.begin();
    for (; it != _bannedUsers.end(); ++it) {
        if (*it == str)
            return true;
    }
    return false;
}
