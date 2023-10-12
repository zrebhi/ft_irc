#include "Command.hpp"

void Command::pass()
{
	std::string incorrect = ":serveur 464 * :Incorrect password.";
	std::string needParams = ":serveur 461 PASS :Not enough parameters.";
	if (_ircServ.isProtected() == false) 
	{
		std::cout << "not isProtected" << std::endl;
		return _client.setRegistered();
	}
	if (_commandArray.size() < 2 || _commandArray[1].length() == 0)
	{
		std::cout << "needParams" << std::endl;
		send(_client.getSocket(), needParams.c_str(), needParams.length(), 0);
	}
	else if (_ircServ.passwordIsValid(_commandArray[1]))
	{
		std::cout << "valid pass" << std::endl;
		return _client.setRegistered();
	}
	else
		send(_client.getSocket(), incorrect.c_str(), incorrect.length(), 0);
}
