/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:50:31 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/18 21:54:25 by zrebhi           ###   ########.fr       */
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
	void	privmsg();
	void	who();
	void	shutdown();
	void	pass();
	void	mode();
	void	part();
	void	invite();
	void	kick();

	bool	registerRequest();

	void	setITKL_Modes(char letterMode, size_t &argIndex);
	void	setO_Modes(size_t &argIndex);
	void	currentModesStr();

private:
	std::vector<std::string> _commandArray;
	Client&	_client;
	Server&	_ircServ;

	void	whoChannel(std::string channelName);

	std::map<std::string, std::string> joinParser();
	void	joinChannel(std::string channelName, std::string channelPassword);
	bool	permissionToJoinChannel(Channel& channel, std::string channelName, std::string channelPassword);

	void	channelMessage();
	void	privateMessage();

	bool	nicknameAvailable(std::string nickname);
	bool	nicknameIsValid(std::string nickname);
	void	changeNicknameInChannels(std::string oldNickname);

	bool	validServerPassword();

	void	createChannel(std::string channelName, std::string password);
	bool	channelExists(std::string channelName);
	bool	IsChannelMember(std::string userNickname, std::string channelName);
	std::map<int, Client>::const_iterator findClientOnServer(const std::string &nickname);
};
