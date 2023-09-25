#include "../inc/Client.hpp"

Client::Client()
{
	this->nickname = "";
	this->ip = "";
	this->socket = 0;
	this->rights = 0;
}

Client::Client(std::string nickname, int socket)
{
	this->nickname = nickname;
	this->socket = socket;
	this->rights = 0;
}

std::string Client::getPseudo()
{
	return this->nickname;
}

std::string Client::getIp()
{
	return this->ip;
}

int Client::getSocket()
{
	return this->socket;
}

void Client::setPseudo(std::string nickname)
{
	this->nickname = nickname;
}

void Client::setIp(std::string ip)
{
	this->ip = ip;
}

void Client::setSocket(int socket)
{
	this->socket = socket;
}

int Client::getRights()
{
	return this->rights;
}

void Client::setRights(int rights)
{
	this->rights = rights;
}
