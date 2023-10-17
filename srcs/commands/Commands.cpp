#include "../inc/CommonLibs.hpp"
#include "../inc/Server.hpp"

// NE SERT PAS POUR L'INSTANT
bool Server::isCommand(std::string input)
{
	static std::string cmds[] = {"KICK", "INVITE", "MODE", "TOPIC", "USER"};
	for (int i = 0; i < CMD_SIZE; i++)
	{
		if (input.compare(cmds[i]))
			return true;
	}
	return false;
}

// t_cmd;
// {
// 	std::string cmdType;  || ex: NICK
// 	std::vector<std::string> params; || ex: {moboigui}
// }
