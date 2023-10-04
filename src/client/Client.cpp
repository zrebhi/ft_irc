/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 21:12:48 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/03 20:17:41 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::Client(int clientSocket) : _clientSocket(clientSocket) {
	this->_clientSocket = clientSocket;
}

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

const std::string Client::getNickname() const {
	return this->_nickname;
}

const std::string Client::getUsername() const {
	return this->_username;
}

const std::string Client::getPassword() const {
	return this->_password;
}

const std::string Client::getHostname() const {
	char	hostname[256];
	gethostname(hostname, sizeof(hostname));
	std::string host(hostname);
	return (host);
}

void Client::setNickname(const std::string &nickname) {
	this->_nickname = nickname;
}

void Client::setUsername(const std::string &username) {
	this->_username = username;
}

void Client::setPassword(const std::string &password) {
	this->_password = password;
}
