/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:57:57 by zrebhi            #+#    #+#             */
/*   Updated: 2023/09/28 23:20:44 by zrebhi           ###   ########.fr       */
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
#include "../client/Client.hpp"

#define portNumber 6667

class Client;

class Server {
public:
	Server();
	~Server();

	void	listenToNewEvents();

private:
	int	_serverSocket;
	std::map<int, Client>	_clientSockets;
	struct sockaddr_in	_serverAddress;

	void	createSocket();
	void	bindSocket();
	void	setSocketToListen ();
	void	acceptConnection();
	void	serverSetup();

	void	pollSetup(struct pollfd *fds);

	void	manageEvents(struct pollfd *fds);
	void	commandHandler(std::string bufferString, Client &client);
	std::vector<std::string>	ft_split(std::string string, char separator);

	Server(const Server &src);
	Server	&operator=(const Server &rhs);
};