#include "Command.hpp"

void Command::pass()
{
	if (_client.isRegistered()  == FULL_REGISTRATION)
		return ft_send(_client, ERR_ALREADYREGISTRED(_client.getNickname()));
	if (!_ircServ.isProtected()) 
	{
		_client.setRegistered(SERV_REGISTRATION);
		std::string clientName = _client.getNickname();
		if (clientName.empty())
			clientName = "guest";
		return ft_send(_client, "Server is not locked by a password");
	}
	if (_commandArray.size() < 2 || _commandArray[1].empty())
		return ft_send(_client, ERR_NEEDMOREPARAMS(_client, _commandArray[0]));
	if (_ircServ.passwordIsValid(_commandArray[1]))
		return _client.setRegistered(SERV_REGISTRATION);
}
