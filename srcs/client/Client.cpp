/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 21:12:48 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 12:51:27 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::Client(int clientSocket) : _clientSocket(clientSocket) {}

Client::~Client() {}

Client &Client::operator=(const Client &rhs) {
	this->_nickname = rhs._nickname;
	this->_password = rhs._password;
	this->_clientSocket = rhs._clientSocket;
	return *this;
}

Client::Client(const Client &src) {
	this->_nickname = src._nickname;
	this->_password = src._password;
	this->_clientSocket = src._clientSocket;
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
	_registered = true;
}

bool Client::getRegistered()
{
	return _registered;
}

bool Client::operator==(std::string nickname) {
	return this->_nickname == nickname;
}
