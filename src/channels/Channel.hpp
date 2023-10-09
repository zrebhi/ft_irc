/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:38:38 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/09 23:58:31 by zrebhi           ###   ########.fr       */
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

	std::string	getName() const;
	std::string getUserList();
	std::map<int, Client> getUsers();

private:
	std::string	_name;
	std::map<int, Client>	_users;
	std::map<int, Client>	_operators;
};