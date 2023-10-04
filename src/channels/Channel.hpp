/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:38:38 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/03 21:04:08 by zrebhi           ###   ########.fr       */
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

	void	addUser(const Client &user);

	void	printUserList();
	std::string	getName() const;
	std::map<int, Client> getUserList() const;

private:
	std::string	_name;
	std::map<int, Client>	_users;
};