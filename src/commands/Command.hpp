/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:50:31 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/10 20:47:25 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include "../client/Client.hpp"
#include "../channels/Channel.hpp"
#include "../server/Server.hpp"

class Client;
class Channel;
class Server;

class Command {
public:
	Command(const std::vector<std::string> &commandArray, Client &client, Server &ircServ);
	~Command();

	void	user();
	void	nick();
	void	join(std::map<std::string, Channel> &channels);
	void	list(std::map <std::string, Channel> &channels);
	void	privmsg();
	void	who();
	void	whoChannel();

private:
	std::vector<std::string> _commandArray;
	Client&	_client;
	Server&	_ircServ;

	void	channelMessage();
	bool	channelExists();
	bool	IsChannelMember();
	void	privateMessage();
};