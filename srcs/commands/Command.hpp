/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:50:31 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 20:29:04 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/Server.hpp"

class Server;
class Channel;

class Command {
public:
	Command(const std::vector<std::string> &commandArray, Client &client, Server &ircServ);
	~Command();

	void	user();
	void	nick();
	void	join();
	void	list(std::map <std::string, Channel> &channels);
	void	privmsg();
	void	who();
	void	whoChannel();
	void	shutdown();
	void	pass();
	void	mode(std::map<std::string, Channel> &channels);

	bool	registerCheck();

private:
	std::vector<std::string> _commandArray;
	Client&	_client;
	Server&	_ircServ;

	void	channelMessage();
	void	privateMessage();

	bool	nicknameAvailable(std::string nickname);
	bool	nicknameIsValid(std::string nickname);

	bool	channelExists(std::string channelName);
	bool	IsChannelMember(std::string userNickname, std::string channelName);
};
