#include "Command.hpp"

void Command::pass()
{
	std::string incorrect = ":IRC 464 * :Incorrect password.";
	std::string needParams = ":IRC 461 PASS :Not enough parameters.";
	if (_ircServ.isProtected() == false) 
		return _client.setRegistered(SERV_REGISTRATION);
	if (_commandArray.size() < 2 || _commandArray[1].length() == 0)
		send(_client.getSocket(), needParams.c_str(), needParams.length(), 0);
	else if (_ircServ.passwordIsValid(_commandArray[1]))
		return _client.setRegistered(SERV_REGISTRATION);
	else
		send(_client.getSocket(), incorrect.c_str(), incorrect.length(), 0);
}

// void Command::pass()
// {
// 	if (this->_commandArray.size() < 2 || this->_commandArray[1].length() == 0)
// 		ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, this->_commandArray[0]));
// 	else
// 		this->_client.setPassword(this->_commandArray[1]);
// }

