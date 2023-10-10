/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:57:57 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/11 00:23:42 by zrebhi           ###   ########.fr       */
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
#include "../utils/Utils.hpp"

class Client;
class Channel;

class Server {
public:
	Server();
	Server(int portNumber);
	Server(const Server &src);
	~Server();

	void	listenToNewEvents();

	std::map<std::string, Channel> getChannelList() const;
	Channel	getChannel(std::string channelName);
	std::vector<Client>	getClientList() const;

	void	addClientToServer(Client &client);

private:
	int	_serverSocket;
	int	_portNumber;
	struct sockaddr_in	_serverAddress;

	std::vector<Client>				_clients;
	std::map<std::string, Channel>	_channels;

	void	createSocket();
	void	bindSocket();
	void	setSocketToListen ();
	void	acceptConnection();
	void	serverSetup();

	void	pollSetup(struct pollfd *fds);

	void	manageEvents(struct pollfd *fds);
	void	commandHandler(std::string bufferString, Client &client);

	Server	&operator=(const Server &rhs);
};
