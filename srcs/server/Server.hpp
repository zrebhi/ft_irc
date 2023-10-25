/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:57:57 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/23 20:04:49 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <ctime>
#include <map>
#include <vector>
#include <sys/epoll.h>
#include <cerrno>
#include "../client/Client.hpp"
#include "../channels/Channel.hpp"
#include "../commands/Command.hpp"
#include "../utils/Utils.hpp"
#include "ServerReplies.hpp"

class Command;

typedef void (Command::*CommandFunction)();

class Server {
public:
	Server();
	Server(int portNumber, std::string &password);
	Server(const Server &src);
	~Server();

	void	listenToNewEvents();

	void	serverShutdown();
	bool	serverRunning();

	std::map<std::string, Channel>&	getChannelList();
	Channel&						getChannel(std::string channelName);
	std::map<int, Client>			&getClientList();
	std::string						getServerPassword();
	int								getServerSocket();
	int								getEpollFd();

	void	addChannelToServer(Channel channel);
	void	removeClientFromServer(Client &client, std::string message);

	bool	isProtected();
	bool	passwordIsValid(const std::string &password);

	bool isFlooding(Client &client);

private:
	int			_portNumber;
	std::string	_password;

	int					_serverSocket;
	int					_epollFd;
	struct sockaddr_in	_serverAddress;

	bool	_serverUp;

	std::map<int, Client>			_clients;
	std::map<std::string, Channel>	_channels;
	std::map<std::string, CommandFunction>	_commandMap;
	std::vector<std::string> _bannedUsers;

	bool isUserBanned(const std::string& str);

	void	createSocket();
	void	bindSocket();
	void	setSocketToListen ();
	void	acceptConnection();
	void	serverSetup();

	void	pollSetup(struct pollfd *fds);
	void	epollSetup();
	void	addSocketToEpoll(int Socket);

	void	manageClientEvents(Client &client);
	void	commandHandler(std::string bufferString, Client &client);
	void	commandMapping();

	bool	validBufferInput(ssize_t bytesRead, std::string bufferInput);

	Server	&operator=(const Server &rhs);
};
