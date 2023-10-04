/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:57:57 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/04 22:38:39 by zrebhi           ###   ########.fr       */
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
#include "../channels/Channel.hpp"
#include "../commands/Command.hpp"

#define portNumber 6667

class Client;
class Channel;

class Server {
public:
	Server();
	Server(const Server &src);
	~Server();

	void	listenToNewEvents();

	std::map<std::string, Channel> getChannelList() const;
	std::map<int, Client>	getClientList() const;

private:
	int	_serverSocket;
	std::map<int, Client>	_clientSockets;
	std::map<std::string, Channel> _channels;
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

	Server	&operator=(const Server &rhs);
};

int	ft_send(Client user, std::string reply);
