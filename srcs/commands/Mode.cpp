#include "Command.hpp"

void	Command::mode(std::map <std::string, Channel> &channels)
{
	//gestion uniquement du mot de passe
	std::string channelName = _commandArray[1];
	std::string password = "NO";
	if (channelName.at(0) == '#')
		channelName = channelName.substr(1);
	if (_commandArray.size() == 4 && _commandArray[2] == "+k")
	{
		password = _commandArray[3];
		if (!channelExists(channelName) || password.empty())
			return;
		channels[channelName].setPassword(password, _client.getNickname());
		std::cout << "mode ok" << std::endl;
	}
}
