#include <iostream>
#include <sys/socket.h>
#ifndef ERRORCODE_HPP

#include "CommonLibs.hpp"
#include "Definition.hpp"

#define ERR_NONICK 431
#define ERR_NICK 432
#define ERR_NICKUSE 433

#define ERR_NEEDPARAMS 461
#define ERR_REGISTERED 462

#define ERR_TOPIC 332
#define ERR_INVITE 341
#define ERR_KICK 342
#define ERR_MODE 324
#define ERR_PORT 667
#define ERR_PASS 464
#define ERR_PRIVMSG 401
#define ERR_PRIVMSG_CHANNEL 404
#define ERR_JOIN 473
#define ERR_LIST 322

class ErrorCodes
{
  public:
	ErrorCodes(const std::string &nickname, const std::string &param, const int socket)
		: _host(HOST), _nickname(nickname), _param(param), _socket(socket)
	{
		errorMessages_[ERR_NICK] = " :incorrect NICK.\n";
		errorMessages_[ERR_NONICK] = " :no NICK given.\n";
		errorMessages_[ERR_NICKUSE] = " :Le pseudonyme est déjà utilisé.\n";

		errorMessages_[ERR_NEEDPARAMS] = " :Need more parameters.\n";
		errorMessages_[ERR_REGISTERED] = " :Already registered.\n";

		errorMessages_[ERR_TOPIC] = " :Le sujet du canal est ";
		errorMessages_[ERR_INVITE] = " :Vous n'avez pas l'autorisation d'inviter des utilisateurs.";
		errorMessages_[ERR_KICK] = " :Vous n'avez pas l'autorisation de chasser un utilisateur.";
		errorMessages_[ERR_MODE] = " :Vous n'avez pas l'autorisation de changer les modes du canal.";
		errorMessages_[ERR_PORT] = " :Le port spécifié n'est pas valide.";
		errorMessages_[ERR_PASS] = " :Mot de passe incorrect.";
		errorMessages_[ERR_PRIVMSG] = " :L'utilisateur spécifié n'existe pas.";
		errorMessages_[ERR_PRIVMSG_CHANNEL] = " :Le canal spécifié n'existe pas.";
		errorMessages_[ERR_JOIN] = " :Vous n'avez pas l'autorisation de rejoindre ce canal.";
		errorMessages_[ERR_LIST] = " :Impossible de récupérer la liste des canaux.";
	}

	std::string sendErrMsg(int code) const
	{
		if (errorMessages_.find(code) != errorMessages_.end())
		{
			std::stringstream message;
			message << ":" << _host << " " << code << " " << _nickname;
			if (_param.empty() == false)
				message << " " << _param;
			message << errorMessages_.at(code);
			send(_socket, message.str().c_str(), message.str().size(), 0);
			return message.str();
		}
		return "Code d'erreur inconnu";
	}

  private:
	std::map<int, std::string> errorMessages_;
	const std::string _host;
	const std::string _nickname;
	const std::string _param;
	const int _socket;
};

#endif // !ERRORCODE_HPP
