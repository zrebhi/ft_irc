#include "Command.hpp"

void	Command::mode(std::map <std::string, Channel> &channels)
{
	//gestion uniquement du mot de passe
	std::string channelName = _commandArray[1];
	std::string password = "";
	if (_commandArray.size() >= 4)
		password = _commandArray[3];
	if (channelName.at(0) == '#')
		channelName = channelName.substr(1);
	if (_commandArray.size() == 4 && _commandArray[2] == "+k")
	{
		std::cout << "one" << std::endl;
		if (!channelExists(channelName) || password.empty())
			return;
		std::cout << "two" << std::endl;
		channels[channelName].setPassword(password, _client.getNickname());
	}
}
