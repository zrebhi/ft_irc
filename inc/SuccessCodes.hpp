#include <map>
#include <string>
#ifndef SUCCESSCODE_HPP

#include "Channels.hpp"
#include "Client.hpp"
#include "CommonLibs.hpp"
#include "Definition.hpp"

#define EMPTY 0
#define BASIC 001
#define CHANNEL 001
#define NAMESSTART 353
#define NAMESEND 366
#define USER_LIST 1000
#define NICK 401
#define NOCHANNEL 403
#define USER 461
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
		_formatRPL[BASIC] = ":<host> <code> <nick> :<message>\n";
		_formatRPL[CHANNEL] = ":<host> <code> <channel> :\n";
		//---------
		_formatMSG[NICK] = "Welcome to ft_irc <nick>\nType FT_HELP for commands\n";
		_formatMSG[USER] = "USER added '<username>' '<realname>' to '<host>'\n";
		_formatMSG[USER_LIST] = "<userlist>";
		_formatRPL[NAMESSTART] = channel.getusersList();
		_formatMSG[NAMESEND] = "End of /NAMES list.";
	}

	~SuccessCodes()
	{
	}

	std::string createStr(int format, int message)
	{
		std::map<int, std::string>::iterator it = _formatRPL.find(format);
		if (it != _formatRPL.end())
		{
			std::string formatted = it->second;
			if (message != EMPTY)
				replacer(formatted, "<message>", _formatMSG.find(message)->second);
			replacer(formatted, "<host>", _host);
			replacer(formatted, "<code>", ft_to_string(message));
			replacer(formatted, "<nick>", _client->getNick());
			replacer(formatted, "<username>", _client->getUsername());
			replacer(formatted, "<realname>", _client->getRealname());
			if (_channel.getChanName().empty() == false)
			{
				replacer(formatted, "<channel>", ("#" + _channel.getChanName()));
				replacer(formatted, "<userlist>", _channel.getusersList());
			}
			// replacer(formatted, "<>", _client->);
			return formatted;
		}
		else
			return "";
	}

	void replacer(std::string &str, const std::string &placeholder, const std::string &replacement)
	{
		if (replacement.empty())
			return;
		size_t pos = 0;
		while ((pos = str.find(placeholder, pos)) != std::string::npos)
		{
			str.replace(pos, placeholder.length(), replacement);
			pos += replacement.length();
		}
	}

  private:
	std::map<int, std::string> _formatRPL;
	std::map<int, std::string> _formatMSG;
	const std::string _host;
	Client *_client;
	Channels &_channel;
	std::string _message;
};

#endif
