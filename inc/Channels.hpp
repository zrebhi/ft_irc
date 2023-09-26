#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "Client.hpp"
#include "CommonLibs.hpp"
#include <string>

class Channels
{
  public:
	Channels();
	Channels(std::string chanName);
	~Channels();
	// setters
	void setMode(int mode);
	void setTopic(std::string newTopic);
	void setChanName(std::string newChanName);
	// getters
	int getMode();
	std::string getTopic();
	std::string getChanName();
	std::string getusersList();
	// functions
	int addUser(Client *newClient);
	bool removeUser(Client *oldClient);
	void invite(std::string nickname);

  private:
	std::vector<Client *> _usersList;
	std::string _chanName;
	std::string _topic;
	int _chanMode;
	// functions
};
#endif // !DEBUG
