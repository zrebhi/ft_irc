/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 21:12:48 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/25 22:28:56 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::Client(int clientSocket)
	: _nickname("*"), _clientSocket(clientSocket) {
	_registered.push_back(false);
	_registered.push_back(false);
	_registered.push_back(false);
}

Client::~Client() {}

Client &Client::operator=(const Client &rhs) {
	this->_nickname = rhs._nickname;
	this->_username = rhs._username;
	this->_realname= rhs._realname;
	this->_password = rhs._password;
	this->_clientSocket = rhs._clientSocket;
	this->_registered = rhs._registered;
	this->_floodClock = rhs._floodClock;
	return *this;
}

Client::Client(const Client &src) {
	this->_nickname = src._nickname;
	this->_username = src._username;
	this->_realname= src._realname;
	this->_password = src._password;
	this->_clientSocket = src._clientSocket;
	this->_registered = src._registered;
	this->_floodClock = src._floodClock;
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

std::string Client::getRealname() const {
	return this->_realname;
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

void Client::setRealname(std::string realname) {
	this->_realname = realname;
}

void Client::setPassword(std::string password) {
	this->_password = password;
}

void Client::setRegistered(int type) {
	bool greetings = false;
	if (_registered.at(type) == false)
		_registered.at(type) = greetings = true;
	for (size_t i = 0; i < _registered.size(); i++)
	{
		if (_registered.at(i) == false)
			return;
	}
	if (greetings)
		ft_send(*this, RPL_WELCOME((*this)));
}

int Client::isRegistered() {
	if (_registered[0] && _registered[1] && _registered[2])
		return FULL_REGISTRATION;
	if (!_registered.at(SERV_REGISTRATION))
		return SERV_REGISTRATION;
	if (!_registered.at(USER_REGISTRATION))
		return USER_REGISTRATION;
	if (!_registered.at(NICK_REGISTRATION))
		return NICK_REGISTRATION;
	return 0;
}

bool Client::operator==(std::string nickname) {
	return this->_nickname == nickname;
}

std::string Client::getBuffer() {
	return this->_buffer;
}

void Client::setBuffer(std::string string) {
	_buffer = string;
}

void Client::appendBuffer(std::string string) {
	_buffer.append(string);
}

void Client::clearBuffer() {
	_buffer.clear();
}

void Client::setFloodCounter(bool increaseOrReset)
{
	if (increaseOrReset == ADD)
		_floodCounter++;
	else if (increaseOrReset == REMOVE)
	{
		_floodCounter = 0;
		setFloodClock();
	}
}

void Client::setFloodClock()
{
	_floodClock = std::time(NULL);
}

int Client::getFloodCounter()
{
	return _floodCounter;
}

time_t Client::getFloodClock()
{
	return _floodClock;
}

