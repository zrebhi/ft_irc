#include "../inc/Client.hpp"

Client::Client()
{
	this->socket = -1;
}

Client::Client(int newSocket)
{
	socket = newSocket;
}

std::string Client::getNickname()
{
	return this->nickname;
}

int Client::getSocket()
{
	return this->socket;
}

void Client::setNickname(std::string nickname)
{
	this->nickname = nickname;
}

void Client::setSocket(int socket)
{
	this->socket = socket;
}

std::string Client::getUsername()
{
	return username;
}

void Client::setUsername(std::string newUsername)
{
	username = newUsername;
}

std::string Client::getRealname()
{
	return username;
}

void Client::setRealname(std::string newRealname)
{
	username = newRealname;
}
