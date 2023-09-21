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

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>

#define portNumber 8080

class Server {
public:
	Server();
	~Server();

	void	createSocket();
	void	bindSocket();
	void	setSocketToListen ();
	void	acceptConnection();
	void	serverSetup();

private:
	int	_serverSocket;
	std::vector<int>	_clientSockets;
	struct sockaddr_in	_serverAddress;

	Server(const Server &src);
	Server	&operator=(const Server &rhs);
};