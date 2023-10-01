#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "CommonLibs.hpp"

class Client
{
  private:
	int _socket;
	std::string _nickname;
	std::string _username;
	std::string _realname;

  public:
	Client();
	Client(int newSocket);
	~Client();
	std::string getNick();
	std::string getIp();
	int getSocket();
	void setNickname(std::string newNickname);
	void setSocket(int newSocket);
	std::string getUsername();
	void setUsername(std::string newUsername);
	std::string getRealname();
	void setRealname(std::string newRealname);
};

#endif // !CLIENT_HPP
