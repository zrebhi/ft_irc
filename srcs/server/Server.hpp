/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:57:57 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/11 21:03:54 by zrebhi           ###   ########.fr       */
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
#include <map>
#include <vector>
#include <sys/epoll.h>
#include <cerrno>
#include "../client/Client.hpp"
#include "../channels/Channel.hpp"
#include "../commands/Command.hpp"
#include "../utils/Utils.hpp"

class Client;
class Channel;

class Server {
public:
	Server();
	Server(int portNumber, std::string &password);
	Server(const Server &src);
	~Server();

	void	listenToNewEvents();

	void	serverShutdown();
	bool	serverRunning();

	std::map<std::string, Channel> getChannelList() const;
	Channel	getChannel(std::string channelName);
	std::map<int, Client>	getClientList() const;
	int	getServerSocket();
	int	getEpollFd();

	void	addClientToServer(Client &client);
	bool	isProtected();
	bool	passwordIsValid(std::string &password);

private:
	int	_serverSocket;
	int	_epollFd;
	int	_portNumber;
	std::string _password;
	struct sockaddr_in	_serverAddress;

	bool	_serverUp;

	std::map<int, Client>			_clients;
	std::map<std::string, Channel>	_channels;

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

	Server	&operator=(const Server &rhs);
};
