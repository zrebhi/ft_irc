#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "CommonLibs.hpp"

class Client
{
  private:
	std::string nickname;
	std::string username;
	std::string realname;
	std::string ip;
	int socket;
	int rights;

  public:
	Client();
	Client(std::string pseudo, int socket);
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
