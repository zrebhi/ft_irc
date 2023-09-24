#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
  private:
	std::string pseudo;
	int socket;
	int rights;

  public:
	Client();
	~Client();
	Client(std::string pseudo, int socket);
	std::string getPseudo();
	int getSocket();
	void setPseudo(std::string pseudo);
	void setSocket(int socket);
	int getRights();
	void setRights(int rights);
};

#endif // !CLIENT_HPP
