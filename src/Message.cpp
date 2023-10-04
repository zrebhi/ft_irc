#include "../inc/Message.hpp"
#include "../inc/Channels.hpp"
#include "../inc/ErrorCodes.hpp"
#include "../inc/SuccessCodes.hpp"
#include <algorithm>
#include <cstring>
#include <exception>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <vector>
#define USERNAME_MAX_LEN 20
#define REALNAME_MAX_LEN 30
#define MAXCHANNELS 5
#define INDEX 0
#define PROTECTED 1
#define CHANMODE 2
#define INDEX_PROTECTED_MODE 3
#define EMPTYSTR ""
#define OP 5

bool isAlnumStr(std::string str)
{
	for (size_t i = 0; i < str.size(); i++)
	{
		if (!isalnum(str.at(i)) && !strchr("_-", str.at(i)))
			return false;
	}
	return true;
}

Client *isClientExist(std::map<int, Client *> clientList, std::string clientName)
{
	std::map<int, Client *>::iterator it = clientList.begin();
	for (; it != clientList.end(); it++)
	{
		if (it->second->getNick() == clientName)
			return it->second;
	}
	return NULL;
}

int Message::channelExist(std::string chanName, t_chanInfos &infos, std::string password)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getChanName() == chanName)
		{
			infos.index = i;
			infos.isFounded = true;
			infos.protection = _channels[i].getIsProtected(password);
			infos.mode = _channels[i].getMode();
			return true;
		}
	}
	infos.isFounded = false;
	return false;
}

Message::Message()
	: _channels(emptyVector), _clients(emptyMap), _nullChannel()
{
}

Message::Message(std::vector<Channels> &channelList, std::map<int, Client *> &clientList)
	: _channels(channelList), _clients(clientList)
{
}

// TOPIC - INVITE - KICK - MODE - PORT - PASS - NICK - PRIVMSG - PRIVMSG (pour un channel) - USER - JOIN - LIST
bool Message::handleInput(Client *client, std::string input)
{
	t_cmd *cmd = parseInput(input);
	if (!cmd)
		return false;
	if (cmd->cmdType.compare("NICK") == 0)
		nickHandler(cmd, client);
	else if (cmd->cmdType.compare("PRIVMSG") == 0)
		messageHandler(cmd, client);
	else if (cmd->cmdType.compare("USER") == 0)
		userHandler(cmd, client);
	else if (cmd->cmdType.compare("JOIN") == 0)
		joinHandler(cmd, client);
	else if (cmd->cmdType.compare("CAP") == 0)
		capHandler(cmd, client->getSocket());
	else if (cmd->cmdType.compare("LIST") == 0)
		listHandler(client->getSocket(), client->getNick());
	else if (cmd->cmdType.compare("MODE") == 0)
		modeHandler(cmd, client);
	else if (cmd->cmdType.compare("TOPIC") == 0)
		topicHandler(cmd, client);
	else if (cmd->cmdType.compare("QUIT") == 0)
		return true;
	// QUIT => envoye un message a tous les canaux ou le client est connecte (via Server a removeClient ?)
	delete cmd;
	return false;
}

t_cmd *Message::parseInput(std::string &input)
{
	t_cmd *cmd = NULL;
	if (!input.empty())
	{
		char lastChar = input.at(input.size() - 1);
		if (lastChar == '\r' || lastChar == '\n')
			input = input.substr(0, input.size() - 1);
	}
	else
		return NULL;
	cmd = new t_cmd;
	size_t spacePos = input.find(' ');
	cmd->cmdType = input.substr(0, spacePos);
	if (spacePos != std::string::npos)
	{
		std::string leftover = input.substr(spacePos + 1);
		size_t wordStart = 0;
		while (wordStart < leftover.length())
		{
			size_t wordEnd = leftover.find(' ', wordStart);
			if (wordEnd == std::string::npos)
				wordEnd = leftover.length();
			cmd->params.push_back(leftover.substr(wordStart, wordEnd - wordStart));
			wordStart = wordEnd + 1;
			if (leftover[wordStart] == ':')
			{
				cmd->params.push_back(leftover.substr(wordStart + 1));
				break;
			}
		}
	}
	return cmd;
	// Format d'envoi : TOPIC #canal :nouveau_sujet
	// Format d'envoi : INVITE utilisateur #canal
	// Format d'envoi : KICK #canal utilisateur :raison_de_l'exclusion
	// Format d'envoi : MODE #canal +mode
	// Format d'envoi : PASS votre_mot_de_passe
	// Format d'envoi : PRIVMSG #canal :votre_message
}

// Traite la commande NICK
void Message::nickHandler(t_cmd *input, Client *client)
{
	// nick_collision
	std::string &newNick = input->params[0];
	if (input->params.empty())
		ErrorCodes(client->getNick(), EMPTYSTR, client->getSocket()).sendErrMsg(ERR_NONICK);
	else if (newNick.length() < 3 || newNick.length() > 12 || !isAlnumStr(newNick))
		ErrorCodes(client->getNick(), newNick, client->getSocket()).sendErrMsg(ERR_NICK);
	else if (newNick == client->getNick() || isClientExist(_clients, newNick))
		ErrorCodes(client->getNick(), newNick, client->getSocket()).sendErrMsg(ERR_NICKUSE);
	else
	{
		std::string userInfos = client->getUserInfos();
		client->setNickname(newNick);
		SuccessCodes(client, _nullChannel).createStr(OKNICK, userInfos);
	}
}

// Commande USER  pour mettre a jour le profil du client
void Message::userHandler(t_cmd *input, Client *client)
{
	// Format d'envoi : USER nom_utilisateur hote serveur :nom_reel
	std::string username = input->params[0];
	if (client->getUsername() != NOT_REGISTERED)
		ErrorCodes(client->getNick(), EMPTYSTR, client->getSocket()).sendErrMsg(ERR_REGISTERED);
	else if (input->params.size() < 4)
		ErrorCodes(client->getNick(), input->cmdType, client->getSocket()).sendErrMsg(ERR_NEEDPARAMS);
	else
	{
		if (username.length() > USERNAME_MAX_LEN)
			username.resize(USERNAME_MAX_LEN);
		client->setUsername(username);
		client->setHost(input->params[1]);
		client->setServer(input->params[2]);
		std::string realname = input->params[3];
		if (realname.at(0) == ':')
			realname.erase(0, 1);
		if (realname.length() > REALNAME_MAX_LEN)
			realname.resize(REALNAME_MAX_LEN);
		client->setRealname(realname);
		SuccessCodes(client, _nullChannel).createStr(USER, EMPTYSTR);
	}
}

void Message::namesHandler(t_cmd *input, Client *client)
{
	t_chanInfos chanInfos;
	if (input->params.empty())
		ErrorCodes(client->getNick(), input->cmdType, client->getSocket()).sendErrMsg(ERR_NEEDPARAMS);
	else
	{
		if (channelExist(input->params[0], chanInfos, EMPTYSTR))
			SuccessCodes(client, _channels[chanInfos.index]).createStr(NAMES_START, EMPTYSTR);
		SuccessCodes(client, _channels[chanInfos.index]).createStr(NAMES_END, EMPTYSTR);
	}
}

void Message::modeHandler(t_cmd *input, Client *client)
{
	t_chanInfos chanInfos;
	std::string chanName = input->params[0];

	if (input->params.empty() || chanName.size() < 2 || chanName.at(0) != '#')
		ErrorCodes(client->getNick(), input->cmdType, client->getSocket()).sendErrMsg(ERR_NEEDPARAMS);
	else if (channelExist(chanName, chanInfos, EMPTYSTR) == false)
		ErrorCodes(client->getNick(), input->cmdType, client->getSocket()).sendErrMsg(NOSUCHCHAN);
	else
		SuccessCodes(client, _channels[chanInfos.index]).createStr(MODE, EMPTYSTR);
}

void Message::topicHandler(t_cmd *input, Client *client)
{
	t_chanInfos chanInfos;
	std::string &chanName = input->params[0];
	if (chanName[0] != '#' || chanName.length() < 4)
		ErrorCodes(client->getNick(), input->cmdType, client->getSocket()).sendErrMsg(ERR_NEEDPARAMS);
	else if (channelExist(input->params[0], chanInfos, EMPTYSTR) == false)
		ErrorCodes(client->getNick(), input->cmdType, client->getSocket()).sendErrMsg(NOSUCHCHAN);
	else
		SuccessCodes(client, _channels[chanInfos.index]).createStr(TOPIC, EMPTYSTR);
}

// Traite la commande JOIN a  partir d'un client
void Message::joinHandler(t_cmd *input, Client *client)
{
	// Format d'envoi : JOIN #canal
	t_chanInfos chanInfos;
	std::string &chanName = input->params[0];
	if (input->params.empty() || chanName.length() < 4 || chanName.at(0) != '#')
		ErrorCodes(client->getNick(), input->cmdType, client->getSocket()).sendErrMsg(ERR_NEEDPARAMS);
	else
	{
		chanName.erase(0, 1);
		if (!channelExist(chanName, chanInfos, EMPTYSTR))
		{
			if (_channels.size() >= MAXCHANNELS)
				return; // Eroor for too many channels - make void return on error
			else
			{
				Channels newChannel(chanName, client);
				_channels.push_back(newChannel);
				client->setNewChan(newChannel.getChanName());
				SuccessCodes(client, newChannel).createStr(JOIN, EMPTYSTR);
				channelExist(chanName, chanInfos, EMPTYSTR);
			}
		}
		else if (chanInfos.protection != -10000000) // check password
		{
			_channels[chanInfos.index].addUser(client);
			SuccessCodes(client, _channels[chanInfos.index]).createStr(JOIN, EMPTYSTR);
		}
		SuccessCodes(client, _channels[chanInfos.index]).createStr(MODE, EMPTYSTR);
		SuccessCodes(client, _channels[chanInfos.index]).createStr(NAMES_START, EMPTYSTR);
		SuccessCodes(client, _channels[chanInfos.index]).createStr(NAMES_END, EMPTYSTR);
	}
}

void Message::canalMessage(t_cmd *input, Client *client)
{
	// :<sender_nick> PRIVMSG <channel_name> :<message_content>
	t_chanInfos chanInfos;
	if (channelExist(input->params[0].substr(1), chanInfos, ""))
		SuccessCodes(client, _channels[chanInfos.index]).createStr(CMSG, input->params[1]);
	else
		ErrorCodes(client->getNick(), input->cmdType, client->getSocket()).sendErrMsg(NOSUCHCHAN);
}

void Message::privateMessage(t_cmd *input, Client *client)
{
	std::map<int, Client *>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		// si client est le meme ???
		if ((*it).second->getNick() == input->params[0])
		{
			std::string templateMessage = ":" + client->getUserInfos() + " PRIVMSG <nick> :" + input->params[1];
			std::string userInfos = SuccessCodes(it->second, _nullChannel).createStr(PMSG, templateMessage);
			return;
		}
		it++;
	}
	SuccessCodes(client, _nullChannel).createStr(NOSUCHNICK, input->params[0]);
}

// Traite les messages prinves PRIVMSG (pas encore les messages du Channel)
void Message::messageHandler(t_cmd *input, Client *client)
{
	// if input empty
	if (input->params[0].at(0) == '#')
		canalMessage(input, client);
	else
		privateMessage(input, client);
}

void Message::listHandler(int clientSocket, std::string nickname)
{
	// Format d'envoi : LIST		(sans params)
	std::string listStart = ":ft_irc 321 " + nickname + " :Liste des canaux :\n";
	std::string listEnd = ":ft_irc 323 " + nickname + " :Fin de liste des canaux :\n";
	std::string channelStr = ":ft_irc 322 " + nickname;

	send(clientSocket, listStart.c_str(), listStart.size(), 0);
	std::vector<Channels>::iterator it = _channels.begin();
	while (it != _channels.end())
	{
		std::string currentChannel = channelStr;
		currentChannel += " #" + it->getChanName() + " " + it->getNbUsers();
		currentChannel += " :" + it->getTopic() + '\n';
		send(clientSocket, currentChannel.c_str(), currentChannel.size(), 0);
		it++;
	}
	send(clientSocket, listEnd.c_str(), listEnd.size(), 0);
}

void Message::capHandler(t_cmd *input, int clientSocket)
{
	std::string req = "CAP ACK :multi-prefix\n";
	std::string capLS = ":server-name CAP * LS :multi-prefix\n";
	if (input->params[0].compare("REQ"))
		send(clientSocket, req.c_str(), req.size(), 0);
	else
		send(clientSocket, capLS.c_str(), capLS.size(), 0);
}

Message::~Message()
{
}
