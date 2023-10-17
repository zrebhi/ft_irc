/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgresse <bgresse@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:50:31 by zrebhi            #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/10/13 11:13:00 by bgresse          ###   ########.fr       */
/*   Updated: 2023/10/13 10:46:55 by moboigui         ###   ########.fr       */
=======
/*   Updated: 2023/10/13 12:06:17 by bgresse          ###   ########.fr       */
>>>>>>> baptiste
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
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
	void	nick(std::map<int, Client> &clientList);
	void	join(std::map<std::string, Channel> &channels);
	void	kick(std::map<std::string, Channel> &channels);
	void	list(std::map <std::string, Channel> &channels);
	void	privmsg();
	void	who();
	void	whoChannel();
	void	shutdown();
	void	pass();
	void	mode(std::map<std::string, Channel> &channels);
	void	names(Channel &channel);

private:
	std::vector<std::string> _commandArray;
	Client&	_client;
	Server&	_ircServ;

	void	channelMessage();
	void	privateMessage();
	bool	channelExists(std::string channelName);
	bool	IsChannelMember(std::string userNickname, std::string channelName);
};
