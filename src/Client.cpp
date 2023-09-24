#include "../inc/Client.hpp"
#include <iostream>

Client::Client()
{
	this->pseudo = "";
	this->socket = 0;
	this->rights = 0;
}

Client::Client(std::string pseudo, int socket)
{
	this->pseudo = pseudo;
	this->socket = socket;
	this->rights = 0;
	std::cout << "client created" << std::endl;
}

Client::~Client()
{
	std::cout << "client destroyed" << std::endl;
}

std::string Client::getPseudo()
{
	return this->pseudo;
}

int Client::getSocket()
{
	return this->socket;
}

void Client::setPseudo(std::string pseudo)
{
	this->pseudo = pseudo;
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
