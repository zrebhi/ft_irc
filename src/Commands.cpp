#include "../inc/CommonLibs.hpp"
#include "../inc/Server.hpp"
#include <cstddef>
#include <string>

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

t_cmd *Server::parseInput(std::string &input)
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
			if (cmd->cmdType.compare("PRIVMSG") == 0 && cmd->params.size() == 1)
			{
				cmd->params.push_back(leftover);
				break;
			}
		}
	}
	else
	{
		cmd = new t_cmd;
		cmd->cmdType = input;
	}
	return cmd;
}
