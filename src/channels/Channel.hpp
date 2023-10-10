/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:38:38 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/11 01:24:34 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <map>
#include "../client/Client.hpp"

class Client;

class Channel {
public:
	Channel();
	Channel(const std::string &channelName);
	~Channel();

	void	addUser(Client &user);
	void	addOperator(Client &user);

	void	sendMessageToChannel(Client sender, std::string message);

	bool	isOperator(const std::string &nickname);

	std::string	getName() const;
	std::map<std::string, Client> getUsers();
	std::string	userListString();

private:
	std::string	_name;
	std::map<std::string, Client>	_users;
	std::map<std::string, Client>	_operators;
};