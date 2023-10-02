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

int Message::isChannelExist(std::string chanName, int (&chanInfo)[3], std::string password)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels[i].getChanName() == chanName)
		{
			chanInfo[0] = i;
			chanInfo[1] = _channels[i].checkProtected(password);
			chanInfo[2] = _channels[i].getMode();
			return true;
		}
	}
	return false;
}

Message::Message()
	: _channels(emptyVector), _clients(emptyMap)
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
	size_t spacePos = input.find(' ');
	if (spacePos != std::string::npos)
	{
		cmd = new t_cmd;
		cmd->cmdType = input.substr(0, spacePos);
		std::string leftover = input.substr(spacePos + 1);
		size_t wordStart = 0;
		while (wordStart < leftover.length())
		{
			size_t wordEnd = leftover.find(' ', wordStart);
			if (wordEnd == std::string::npos)
				wordEnd = leftover.length();
			cmd->params.push_back(leftover.substr(wordStart, wordEnd - wordStart));
			wordStart = wordEnd + 1;
		}
	}
	else
	{
		cmd = new t_cmd;
		cmd->cmdType = input;
	}
	return cmd;
	// Format d'envoi : TOPIC #canal :nouveau_sujet
	// Format d'envoi : INVITE utilisateur #canal
	// Format d'envoi : KICK #canal utilisateur :raison_de_l'exclusion
	// Format d'envoi : MODE #canal +mode
	// Format d'envoi : PASS votre_mot_de_passe
	// Format d'envoi : PRIVMSG #canal :votre_message
}

std::string createResponse(t_cmd *input, std::string nickname)
{
	std::string response;

	response = nickname + "@" + HOST + "PRIVMSG" + input->params[0] + " :" + input->params[1] + "\n";
	return response;
}

// Traite la commande NICK
void Message::nickHandler(t_cmd *input, Client *client)
{
	std::string strSended;
	// nick_collision
	std::string &newNick = input->params[0];
	if (input->params.empty())
		strSended = ErrorCodes(client->getNick(), "", client->getSocket()).sendErrMsg(ERR_NONICK);
	else if (newNick.length() < 3 || newNick.length() > 12 || !isAlnumStr(newNick))
		strSended = ErrorCodes(client->getNick(), newNick, client->getSocket()).sendErrMsg(ERR_NICK);
	else if (newNick == client->getNick() || isClientExist(_clients, newNick))
		strSended = ErrorCodes(client->getNick(), newNick, client->getSocket()).sendErrMsg(ERR_NICKUSE);
	else
	{
		Channels nullChan;
		// create nullChan dans le serveur et le passer aux classes
		client->setNickname(newNick);
		strSended = SuccessCodes(client, nullChan).createStr(BASIC, NICK);
		std::cout << RED << strSended << RESET << std::endl;
		send(client->getSocket(), strSended.c_str(), strSended.size(), 0);
	}
	std::cout << RED << "Server send : " << strSended << RESET << std::endl;
}

// Commande USER  pour mettre a jour le profil du client
void Message::userHandler(t_cmd *input, Client *client)
{
	// Format d'envoi : USER nom_utilisateur hote serveur :nom_reel
	std::string strSended = ":ft_irc 001 ";
	std::string &username = input->params[0];
	if (client->getUsername() != NOT_REGISTERED)
		strSended = ErrorCodes(client->getNick(), "", client->getSocket()).sendErrMsg(ERR_REGISTERED);
	else if (input->params.size() < 4 || input->params[3].at(0) != ':' || username.empty())
		strSended = ErrorCodes(client->getNick(), input->cmdType, client->getSocket()).sendErrMsg(ERR_NEEDPARAMS);
	else
	{
		Channels nullChan;
		if (username.length() > USERNAME_MAX_LEN)
			username.resize(USERNAME_MAX_LEN);
		client->setUsername(username);
		client->setHost(input->params[1]);
		client->setServer(input->params[2]);
		std::string realname;
		for (size_t i = 3; i < input->params.size() && realname.length() < 30; i++)
		{
			// ne pas decouper le reste de la string si USER dans parseInput
			if (i != 3)
				realname.push_back(' ');
			else if (input->params[i].at(0) == ':')
				input->params[i].erase(0);
			realname.append(input->params[i]);
			if (realname.length() > REALNAME_MAX_LEN)
				realname.resize(REALNAME_MAX_LEN);
		}
		client->setRealname(realname);
		strSended = SuccessCodes(client, nullChan).createStr(BASIC, USER);
		send(client->getSocket(), strSended.c_str(), strSended.size(), 0);
	}
	std::cout << RED << "Server send: " << strSended << RESET << std::endl;
}

void Message::namesHandler(t_cmd *input, Client *client)
{
	std::string strSended = ":ft_irc 353 " + client->getNick() + " :";
	std::string endNames = ":ft_irc 366  # :End of /NAMES list.\n";
	//  boucle pour traiter tous les salons si liste
	int channelInfo[3];
	if (input->params.empty())
		strSended = ErrorCodes(client->getNick(), input->cmdType, client->getSocket()).sendErrMsg(ERR_NEEDPARAMS);
	else if (isChannelExist(input->params[0], channelInfo, ""))
	{
		// std::string userList;
		// strSended += _channels[channelInfo[0]].getusersList();
		strSended = SuccessCodes(client, _channels[channelInfo[0]]).createStr(NAMESSTART, USER_LIST);
		send(client->getSocket(), strSended.c_str(), strSended.size(), 0);
		std::cout << RED << "Server send: " << strSended << RESET << std::endl;
		strSended = SuccessCodes(client, _channels[channelInfo[0]]).createStr(NAMESEND, USER_LIST);
		send(client->getSocket(), strSended.c_str(), strSended.size(), 0);
		std::cout << RED << "Server send: " << strSended << RESET << std::endl;
		// endNames.insert(14, _channels[channelInfo[0]].getChanName());
		// endNames.insert(12, client->getNick());
		// send(client->getSocket(), endNames.c_str(), endNames.size(), 0);
		// std::cout << RED << "Server send: " << endNames << RESET << std::endl;
	}
	else
	{
		strSended = ":ft_irc 403  # :No such channel\n";
		strSended.insert(14, _channels[channelInfo[0]].getChanName());
		strSended.insert(12, client->getNick());
		send(client->getSocket(), strSended.c_str(), strSended.size(), 0);
		std::cout << RED << "Server send: " << strSended << RESET << std::endl;
	}
}

std::string joinStrMake(Client *client, std::string channel)
{
	std::string joinedstr = ":" + client->getNick() + "!" + client->getUsername();
	joinedstr += "@" + client->getHost() + " JOIN " + channel;
	return joinedstr;
}
// Traite la commande JOIN a  partir d'un client
void Message::joinHandler(t_cmd *input, Client *client)
{
	// Format d'envoi : JOIN #canal
	std::string chanName = input->params[0];
	std::string strSended = ":ft_irc 001 ";
	int channelInfo[3];
	if (input->params.empty())
		strSended = ErrorCodes(client->getNick(), input->cmdType, client->getSocket()).sendErrMsg(ERR_NEEDPARAMS);
	else if (!isChannelExist(chanName, channelInfo, ""))
	{
		Channels newChannel(chanName, client);
		_channels.push_back(newChannel); // add client
		_channels[_channels.size() - 1].addUser(client);
		strSended = joinStrMake(client, chanName);
		send(client->getSocket(), strSended.c_str(), strSended.size(), 0);
		std::cout << RED << "Server send: " << strSended << RESET << std::endl;
		std::string strSended = ":ft_irc MODE #" + chanName + " +nt" + "\n";
		send(client->getSocket(), strSended.c_str(), strSended.size(), 0);
		std::cout << RED << "Server send: " << strSended << RESET << std::endl;
		namesHandler(input, client);
	}
	else if (channelInfo[1] == -1)
	{
		_channels[channelInfo[0]].addUser(client);
	}
	std::cout << RED << "Server send: " << strSended << RESET << std::endl;
}

// Traite les messages prinves PRIVMSG (pas encore les messages du Channel)
void Message::messageHandler(t_cmd *input, Client *client)
{
	// Format d'envoi : PRIVMSG destinataire :votre_message
	std::map<int, Client *>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (it->second->getNick().compare(input->params[0]) == 0)
		{
			std::string response = createResponse(input, client->getNick());
			send(it->second->getSocket(), response.c_str(), response.size(), 0);
			return;
		}
		it++;
	}
	std::string notFound = "Error: Client not found.\n";
	send(client->getSocket(), notFound.c_str(), notFound.size(), 0);
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
