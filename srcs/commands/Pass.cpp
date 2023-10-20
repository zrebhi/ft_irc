#include "Command.hpp"

void Command::pass()
{
	if (_commandArray.size() < 2 || _commandArray[1].empty())
		return ft_send(_client, ERR_NEEDMOREPARAMS(_client, _commandArray[0]));

	if (_ircServ.isProtected() == false) 
		return _client.setRegistered(SERV_REGISTRATION);
	else if (_ircServ.passwordIsValid(_commandArray[1]))
		return _client.setRegistered(SERV_REGISTRATION);
	else
		ft_send(_client, ERR_PASSWDMISMATCH);
}
