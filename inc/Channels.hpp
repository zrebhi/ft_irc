#ifndef CHANNELS_HPP
#define CHANNELS_HPP

#include "Client.hpp"
#include "CommonLibs.hpp"
#include <map>

#define INVITE_ONLY 0
#define TOPIC_CMD 1
#define CHAN_KEY 2
#define OP_PRIVILEGE 3
#define USER_LIMIT 4

class Channels
{
  public:
	Channels();
	Channels(std::string chanName);
	Channels(std::string chanName, Client *client);
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
	std::string getNbUsers();
	int getIsProtected(const std::string &password);
	std::vector<Client *> &getUsers();
	// functions
	int addUser(Client *newClient);
	bool removeUser(Client *oldClient);
	void invite(std::string nickname);

  private:
	std::map<Client *, int> _clientList; // client - rights
	std::vector<Client *> _usersList;
	std::vector<Client *> _invited;
	std::vector<Client *> _operators;
	std::vector<Client *> _halfOperators;
	std::string _chanName;
	int _chanMode;
	bool _isProtected;
	std::string _topic;
	std::string _password;
	// functions
};
#endif // !DEBUG
