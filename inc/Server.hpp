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
#include "CommonLibs.hpp"

void virtualClient();

class Server
{
  public:
	Server();
	~Server();
	Server(unsigned int port);

	void createSocket();
	void bindSocket();
	void setSocketToListen();
	void acceptConnection(int epfd);
	void serverSetup();

  private:
	unsigned int portNumber;
	std::vector<Client> _clientList;
	struct pollfd *_fdList;
	bool _serverIsUp;
	int _serverSocket;
	size_t _usersOnline;
	std::map<int, Client *> _clientSockets;
	struct sockaddr_in _serverAddress;
	void parseString(char *receivedMessage);
	struct epoll_event _serverEvent;
	int _epfd;

	void handleClient(Client *client);
	t_cmd *parseInput(const std::string &input);
	bool isCommand(std::string input);
	Server(const Server &src);
	Server &operator=(const Server &rhs);
};

#endif // !SERVER_HPP
