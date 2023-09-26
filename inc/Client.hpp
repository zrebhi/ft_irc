#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "CommonLibs.hpp"

class Client
{
  private:
	std::string nickname;
	std::string username;
	std::string realname;
	std::string topic;
	int socket;

  public:
	Client();
	Client(int socket);
	std::string getNickname();
	std::string getIp();
	int getSocket();
	void setNickname(std::string pseudo);
	void setSocket(int socket);
	std::string getUsername();
	void setUsername(std::string newUsername);
	std::string getRealname();
	void setRealname(std::string newRealname);
};

#endif // !CLIENT_HPP
