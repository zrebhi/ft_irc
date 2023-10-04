#include <map>
#include <string>
#include <sys/socket.h>
#include <vector>
#ifndef SUCCESSCODE_HPP

#include "Channels.hpp"
#include "Client.hpp"
#include "CommonLibs.hpp"
#include "Definition.hpp"

#define EMPTY 0
#define BASIC 001
#define USER_LIST 1000
#define JOIN 1001
#define MODE 1002
#define CHANNEL 1003
/// codes RPL RFC
#define NAMES_START 353
#define NAMES_END 366
#define NOSUCHCHAN 403
#define NICK 401
#define NOCHANNEL 403
#define USER 461
#define WHO 352
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

	void fillFomats()
	{
		_formatRPL[NICK] = ":<host> 001 <nick> :Welcome to ft_irc <nick>\nType FT_HELP for commands\n";
		_formatRPL[USER] = ":<host> 001 <nick> :USER added '<username>' '<realname>' to '<host>'\n";
		_formatRPL[NAMES_START] = ":<host> 353 <nick> = <channel> :<userlist>\n";
		_formatRPL[NAMES_END] = "<host> 356 <nick> <channel> :End of /NAMES list.\n";
		_formatRPL[JOIN] = ":<nick>!<username>@<userhost> JOIN <channel>\n";
		_formatRPL[MODE] = ":<host> MODE <channel> +nt\n"; //+nt modes a gerer
														   // nickname + "@" + HOST + "PRIVMSG" + input->params[0] + " :" + input->params[1] + "\n";

		//---------
	}

	void announceToChannel(std::string announcement)
	{
		std::vector<Client *> &clientList = _channel.getUsers();
		std::vector<Client *>::iterator clientIt = clientList.begin();
		while (clientIt != clientList.end())
			send((*(clientIt++))->getSocket(), announcement.c_str(), announcement.size(), 0);
		std::cout << RED << "Server send to channel: " << announcement << RESET << std::endl;
	}

	std::string createStr(int format, std::string message)
	{
		std::map<int, std::string>::iterator it = _formatRPL.find(format);
		if (it != _formatRPL.end())
		{
			std::string formatted = it->second;
			replacer(formatted, "<host>", _host);
			replacer(formatted, "<nick>", _client->getNick());
			replacer(formatted, "<username>", _client->getUsername());
			replacer(formatted, "<userhost>", _client->getHost());
			replacer(formatted, "<realname>", _client->getRealname());
			replacer(formatted, "<message>", message);
			replacer(formatted, "<channel>", ("#" + _channel.getChanName()));
			replacer(formatted, "<userlist>", _channel.getusersList());
			// replacer(formatted, "<>", _client->);
			if (format == JOIN)
				announceToChannel(formatted);
			else
				send(_client->getSocket(), formatted.c_str(), formatted.size(), 0);
			// {
			// 	send(_client->getSocket(), formatted.c_str(), formatted.size(), 0);
			// 	std::cout << RED << "Server send: " << formatted << RESET << std::endl;
			// }
			// else
			return formatted;
		}
		else
			return "";
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

  private:
	std::map<int, std::string> _formatRPL;
	const std::string _host;
	Client *_client;
	Channels &_channel;
	std::string _message;
};

#endif
