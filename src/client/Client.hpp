/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 21:02:50 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/03 20:16:53 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/Server.hpp"
#include <unistd.h>

class Client {
public:
	Client();
	Client(int clientSocket);
	Client(const Client &src);
	~Client();
	Client	&operator=(const Client &rhs);

	int			getSocket() const;

	const std::string	getNickname() const;
	const std::string	getUsername() const;
	const std::string	getPassword() const;
	const std::string	getHostname() const;

	void	setNickname(const std::string &nickname);
	void	setUsername(const std::string &username);
	void	setPassword(const std::string &password);


private:
	std::string _nickname;
	std::string _password;
	std::string _username;
	std::string _hostname;
	int	_clientSocket;
};