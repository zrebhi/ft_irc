/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 21:12:48 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 20:47:02 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::Client(int clientSocket) : _registered(false), _clientSocket(clientSocket) {}

Client::~Client() {}

Client &Client::operator=(const Client &rhs) {
	this->_nickname = rhs._nickname;
	this->_password = rhs._password;
	this->_clientSocket = rhs._clientSocket;
	this->_registered = rhs._registered;
	return *this;
}

Client::Client(const Client &src) {
	this->_nickname = src._nickname;
	this->_password = src._password;
	this->_clientSocket = src._clientSocket;
	this->_registered = src._registered;
}

int Client::getSocket() const {
	return this->_clientSocket;
}

std::string Client::getNickname() const {
	return this->_nickname;
}

std::string Client::getUsername() const {
	return this->_username;
}

std::string Client::getPassword() const {
	return this->_password;
}

std::string Client::getHostname() const {
	char	hostname[256];
	gethostname(hostname, sizeof(hostname));
	std::string host(hostname);
	return (host);
}

void Client::setNickname(std::string nickname) {
	this->_nickname = nickname;
}

void Client::setUsername(std::string username) {
	this->_username = username;
}

void Client::setPassword(std::string password) {
	this->_password = password;
}

void Client::setRegistered()
{
	ft_send(*this, RPL_WELCOME((*this)));
	_registered = true;
}

bool Client::isRegistered() {
	return _registered;
}

bool Client::operator==(std::string nickname) {
	return this->_nickname == nickname;
}
