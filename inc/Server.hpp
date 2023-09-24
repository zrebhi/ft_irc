/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:57:57 by zrebhi            #+#    #+#             */
/*   Updated: 2023/09/21 19:50:47 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <map>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

void virtualClient(std::string response);

class Server
{
  public:
	Server();
	~Server();
	Server(unsigned int port);

	void createSocket();
	void bindSocket();
	void setSocketToListen();
	void acceptConnection(size_t i);
	void serverSetup();

  private:
	unsigned int portNumber;
	std::map<int, Client *> _clientList;
	struct pollfd *_fdList;
	bool _serverIsUp;
	int _serverSocket;
	size_t _usersOnline;
	std::vector<int> _clientSockets;
	struct sockaddr_in _serverAddress;

	bool nickIsExisting(char *buffer);
	void setNickname(size_t i);
	void ClientInput(size_t i);
	Server(const Server &src);
	Server &operator=(const Server &rhs);
};

#endif // !SERVER_HPP
