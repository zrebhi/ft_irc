#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP

#include "Channels.hpp"
#include "Client.hpp"

#define HOST "ft_irc"

typedef struct s_cmd
{
	std::string cmdType;
	std::vector<std::string> params;
	Client *sender;
	Client *receiver;
	Channels channel;
	std::string username;
	std::string realname;
} t_cmd;

#endif // !DEFINITIONS_HPP
