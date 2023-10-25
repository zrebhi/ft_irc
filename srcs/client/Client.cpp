/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 21:12:48 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/23 20:14:16 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() {}

Client::Client(int clientSocket)
	: _nickname("*"), _clientSocket(clientSocket), _floodClock(std::time(NULL)), _floodCounter(-1) {
	//double construction lors de nick - a verifier
	std::cout << "constructeur client :" << clientSocket << std::endl;
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
	if (type == NICK_REGISTRATION)
		_registered.first = true;
	else if (type == SERV_REGISTRATION)
		_registered.second = true;
	if (_registered.first && _registered.second)
	{
		if (_username.empty() || _realname.empty())
		{
			std::string nb;
			for (int i = 0; i < 10; i++)
				nb.push_back(rand() % 10 + '0');
			if (_username.empty())
				_username = "Guest" + nb;
			if (_realname.empty())
				_realname = "Name" + nb;
		}
		ft_send(*this, RPL_WELCOME((*this)));
	}
}

int Client::isRegistered() {
	if (_registered.first && _registered.second)
		return FULL_REGISTRATION;
	if (_registered.first)
		return NICK_REGISTRATION;
	if (_registered.second)
		return SERV_REGISTRATION;
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

