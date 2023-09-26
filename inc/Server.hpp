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

#include "Channels.hpp"
#include "Client.hpp"
#include "CommonLibs.hpp"

void virtualClient(char letter);

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
	// variables
	unsigned int portNumber;
	bool _serverIsUp;
	int _serverSocket;
	std::map<int, Client *> _clients;
	std::vector<Channels> _channels;
	struct sockaddr_in _serverAddress;
	int _epfd;
	// functions
	void joinHandler(t_cmd *input, Client *client);
	void userHandler(t_cmd *input, Client *client);
	void messageHandler(t_cmd *input, Client *client);
	void nickHandler(t_cmd *input, Client *client);
	void epollSetup();
	void handleClient(Client *client);
	t_cmd *parseInput(const std::string &input);
	bool isCommand(std::string input);
	Server(const Server &src);
};

#endif // !SERVER_HPP
