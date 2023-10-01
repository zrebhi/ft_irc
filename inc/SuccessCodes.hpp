#ifndef SUCCESSCODE_HPP

#include "CommonLibs.hpp"
#include "Definition.hpp"

#define OK_NICK 401
#define OK_TOPIC 332
#define OK_INVITE 341
#define OK_KICK 342
#define OK_MODE 324
#define OK_PORT 667
#define OK_PASS 464
#define OK_PRIVMSG 401
#define OK_PRIVMSG_CHANNEL 404
#define OK_USER 461
#define OK_JOIN 473
#define OK_LIST 322
// NICK OK => :monNick!~monUserNa@2a01:e0a:16e:a460:5eb4:15ad:7f38:78fd NICK :july << PING LAG1695

class SuccessCodes
{
  public:
	SuccessCodes(const std::string &nickname, const std::string &receiver)
		: _host(HOST), _nickname(nickname), _receiver(receiver)
	{
		successMessages_[OK_NICK] = " :Welcome to the server" + _host;
		successMessages_[OK_TOPIC] = " :Le sujet du canal est ";
		successMessages_[OK_INVITE] = " :Vous n'avez pas l'autorisation d'inviter des utilisateurs.";
		successMessages_[OK_KICK] = " :Vous n'avez pas l'autorisation de chasser un utilisateur.";
		successMessages_[OK_MODE] = " :Vous n'avez pas l'autorisation de changer les modes du canal.";
		successMessages_[OK_PORT] = " :Le port spécifié n'est pas valide.";
		successMessages_[OK_PASS] = " :Mot de passe incorrect.";
		successMessages_[OK_PRIVMSG] = " :L'utilisateur spécifié n'existe pas.";
		successMessages_[OK_PRIVMSG_CHANNEL] = " :Le canal spécifié n'existe pas.";
		successMessages_[OK_USER] = " :Paramètres de l'utilisateur incorrects.";
		successMessages_[OK_JOIN] = " :Vous n'avez pas l'autorisation de rejoindre ce canal.";
		successMessages_[OK_LIST] = " :Impossible de récupérer la liste des canaux.";
	}

	std::string getSuccessMessage(int code) const
	{
		if (successMessages_.find(code) != successMessages_.end())
		{
			std::stringstream message;
			message << ":" << _host << " " << code << " " << _nickname << " ";
			if (_receiver.empty() == false)
				message << _receiver << " ";
			message << ":" << successMessages_.at(code);
			return message.str();
		}
		return "Code d'erreur inconnu";
	}

  private:
	std::map<int, std::string> successMessages_;
	const std::string _host;
	const std::string _nickname;
	const std::string _receiver;
};

#endif
