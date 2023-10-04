#include "../inc/Client.hpp"
#include <iterator>
#include <vector>

Client::Client()
	: _chanList()
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

void Client::setHost(std::string newHost)
{
	_host = newHost;
}
void Client::setServer(std::string newServer)
{
	_server = newServer;
}
void Client::setSocket(int newSocket)
{
	_socket = newSocket;
}

void Client::setNewChan(std::string &newChan)
{
	_chanList.push_back(newChan);
}

std::string Client::getHost()
{
	return _host;
}
std::string Client::getServer()
{
	return _server;
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
