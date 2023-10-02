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
	std::string _host;
	std::string _server;

  public:
	Client();
	Client(int newSocket);
	~Client();
	// setters
	void setSocket(int newSocket);
	void setHost(std::string newHost);
	void setServer(std::string newServer);
	void setNickname(std::string newNickname);
	void setUsername(std::string newUsername);
	void setRealname(std::string newRealname);
	// getters
	int getSocket();
	std::string getNick();
	std::string getHost();
	std::string getServer();
	std::string getUsername();
	std::string getRealname();
};

#endif // !CLIENT_HPP
