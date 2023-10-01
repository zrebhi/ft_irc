#include "../inc/Client.hpp"

Client::Client()
{
	_socket = -1;
}

Client::Client(int newSocket)
	: _socket(newSocket), _username(NOT_REGISTERED)
{
}

Client::~Client()
{
	close(_socket);
}

std::string Client::getNick()
{
	return _nickname;
}

int Client::getSocket()
{
	return _socket;
}

void Client::setNickname(std::string newNickname)
{
	_nickname = newNickname;
}

void Client::setSocket(int newSocket)
{
	_socket = newSocket;
}

std::string Client::getUsername()
{
	return _username;
}

void Client::setUsername(std::string newUsername)
{
	_username = newUsername;
}

std::string Client::getRealname()
{
	return _username;
}

void Client::setRealname(std::string newRealname)
{
	_username = newRealname;
}
