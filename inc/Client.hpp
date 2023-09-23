#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
  private:
	std::string pseudo;
	std::string ip;
	int socket;
	int rights;

  public:
	Client();
	Client(std::string pseudo, std::string ip, int socket);
	std::string getPseudo();
	std::string getIp();
	int getSocket();
	void setPseudo(std::string pseudo);
	void setIp(std::string ip);
	void setSocket(int socket);
	int getRights();
	void setRights(int rights);
};

#endif // !CLIENT_HPP
