/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 21:02:50 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/20 19:57:28 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../server/Server.hpp"
#include <unistd.h>
#include <utility>

class Client {
public:
	Client();
	Client(int clientSocket);
	Client(const Client &src);
	~Client();
	Client	&operator=(const Client &rhs);
	bool	operator==(std::string nickname);

	int			getSocket() const;

	bool	isRegistered();
	std::string	getNickname() const;
	std::string	getUsername() const;
	std::string	getPassword() const;
	std::string	getHostname() const;

	void	setRegistered(bool type);
	void	setNickname(std::string nickname);
	void	setUsername(std::string username);
	void	setPassword(std::string password);

	std::string	getBuffer();
	void		setBuffer(std::string string);
	void		appendBuffer(std::string string);
	void		clearBuffer();


private:
	std::pair<bool, bool> _registered;
	std::string _nickname;
	std::string _password;
	std::string _username;
	std::string _hostname;
	std::string _buffer;

	int	_clientSocket;
};
