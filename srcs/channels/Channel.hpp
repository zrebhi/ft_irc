/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:38:38 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 22:40:08 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include "../server/Server.hpp"

class Client;

class Channel {
public:
	Channel();
	~Channel();
	Channel(const std::string &channelName);
	Channel(const Channel &src);
	Channel	&operator=(const Channel &rhs);


	void	addUser(Client &user);
	void	addOperator(Client &user);

	void	sendMessageToChannel(Client sender, std::string message);

	bool	checkPassword(const std::string &password);
	void	setPassword(const std::string &password, const std::string &name);

	bool	isOperator(const std::string &nickname);

	std::string	getName() const;
	std::map<std::string, Client> getUsers();
	std::string	userListString();

private:
	std::string	_name;
	std::string _password;
	std::map<std::string, Client>	_users;
	std::map<std::string, Client>	_operators;
};
