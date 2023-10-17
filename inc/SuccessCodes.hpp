#ifndef SUCCESSCODE_HPP

#include "Channels.hpp"
#include "Client.hpp"
#include "CommonLibs.hpp"
#include "Definition.hpp"

// ===================UTILISATION===================
//
// regarder sur la rfc le code et la structure du message
// -------------------- ex: ------------------------
// Command: JOIN
//
// vous avez la liste des message possible
// ex: (ERR_NEEDMOREPARAMS)        ERR_BANNEDFROMCHAN
// ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
// ERR_CHANNELISFULL               ERR_BADCHANMASK
// ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
// RPL_TOPIC
//
// allez chercher le code et la structure qui vous interesse en bas de la rfc:
//		461     ERR_NEEDMOREPARAMS
//		"<command> :Not enough parameters"
//
// vous creez un define qui correspond au code d'erreur
//		#define NEED_PARAMS 461
//
//			la fonction :std::string createStr(int format, std::string message)
// vous ajouter le format dans fillFomats() avec le define (host est facultatif)
//		_formatRPL[NEED_PARAMS] = "<host> <code>(ou 461) :<message>(2eme parametre de la fonction);
//		et toujours un '\n' a la fin
//
//	dans votre code vous appellez SuccessCodes(client, channel)->createStr(format, message);
//	il va creer la string, l'envoyer a hexchat et afficher la string sur le terminal
//		(dans Message vous avez un _nullChannel si vous n'avez pas besoin du channel)
//
// ===================UTILISATION===================

#define EMPTY 0
#define BASIC 1
#define USER_LIST 8
#define JOIN 1
#define MODE 2
#define CHANNEL 3
#define PMSG 4
#define CMSG 5
#define USERINFOS 6
#define NICK_CHANGE 7
/// codes RPL RFC
#define NAMES_START 353
#define NAMES_END 366
#define NOSUCHCHAN 403
#define NOSUCHNICK 401
#define NOCHANNEL 403
#define USER 461
#define WHO 352
#define TOPIC 332
#define OKNICK 001
// #define TOPIC 332
// #define INVITE 341
// #define KICK 342
// #define MODE 324
// #define PORT 667
// #define PASS 464
// #define PRIVMSG 401
// #define PRIVMSG_CHANNEL 404
// #define JOIN 473
// #define LIST 322

std::string ft_to_string(int nb)
{
	std::stringstream converted;
	converted << nb;
	return converted.str();
}

class SuccessCodes
{
  public:
	SuccessCodes(Client *client, Channels &channel)
		: _host(HOST), _client(client), _channel(channel)
	{
		fillFomats();
	}

	~SuccessCodes()
	{
	}

	std::string createStr(int format, std::string message)
	{
		std::map<int, std::string>::iterator it = _formatRPL.find(format);
		if (it != _formatRPL.end())
		{
			std::string formatted = it->second;
			replacer(formatted, "<message>", message);
			replacer(formatted, "<host>", _host);
			replacer(formatted, "<nick>", _client->getNick());
			replacer(formatted, "<username>", _client->getUsername());
			replacer(formatted, "<userhost>", _client->getHost());
			replacer(formatted, "<realname>", _client->getRealname());
			replacer(formatted, "<channel>", ("#" + _channel.getChanName()));
			replacer(formatted, "<userlist>", _channel.getusersList());
			replacer(formatted, "<userinfos>", _client->getUserInfos());
			replacer(formatted, "<topic>", _channel.getTopic());
			// replacer(formatted, "<>", _client->);
			if (format == JOIN)
				announceToChannel(formatted, true);
			else if (format == CMSG)
				announceToChannel(formatted, false);
			else
				send(_client->getSocket(), formatted.c_str(), formatted.size(), 0);
			std::cout << RED << "Server send: " << formatted << RESET << std::endl;
			return formatted;
		}
		else
			return "";
	}

  private:
	std::map<int, std::string> _formatRPL;
	const std::string _host;
	Client *_client;
	Channels &_channel;
	std::string _message;
	void fillFomats()
	{
		_formatRPL[OKNICK] = ":<host> 001 <nick> :Welcome to ft_irc <nick>\nType FT_HELP for commands\n";
		_formatRPL[NOSUCHNICK] = ":<host> 401 <nick> :No such nick(<message>).\n";
		_formatRPL[NICK_CHANGE] = ":<message> NICK :<nick>";
		_formatRPL[USER] = ":<host> 001 <nick> :USER added '<username>' '<realname>' to '<host>'\n";
		_formatRPL[NAMES_START] = ":<host> 353 <nick> = <channel> :<userlist>\n";
		_formatRPL[NAMES_END] = "<host> 356 <nick> <channel> :End of /NAMES list.\n";
		_formatRPL[JOIN] = ":<userinfos> JOIN <channel>\n";
		_formatRPL[MODE] = ":<host> MODE <channel> +nt\n"; //+nt modes a gerer
		_formatRPL[CMSG] = ":<userinfos> PRIVMSG <channel> :<message>\n";
		_formatRPL[PMSG] = "<message>\n";
		_formatRPL[TOPIC] = ":<host> 332 <nick> <channel> :<topic>\n";
		//---------
	}

	void announceToChannel(std::string announcement, bool selfSend)
	{
		std::vector<Client *> &clientList = _channel.getUsers();
		std::vector<Client *>::iterator clientIt = clientList.begin();
		while (clientIt != clientList.end())
		{
			if (selfSend || (*clientIt)->getNick() != _client->getNick())
				send((*(clientIt))->getSocket(), announcement.c_str(), announcement.size(), 0);
			clientIt++;
		}
		std::cout << RED << "Server send to channel: " << announcement << RESET << std::endl;
	}

	void replacer(std::string &str, const std::string &placeholder, const std::string &replacement)
	{
		std::string newStr;
		if (replacement.empty())
			return;
		if (replacement == "0" && placeholder == "userhost")
			newStr = "undefined";
		else
			newStr = replacement;
		size_t pos = 0;
		while ((pos = str.find(placeholder, pos)) != std::string::npos)
		{
			str.replace(pos, placeholder.length(), newStr);
			pos += newStr.length();
		}
	}
};

#endif
