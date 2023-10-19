#include "Command.hpp"

void Command::pass()
{
	if (_ircServ.isProtected() == false) 
		return _client.setRegistered(SERV_REGISTRATION);
	if (_commandArray.size() < 2 || _commandArray[1].length() == 0)
		ft_send(_client, ERR_NEEDMOREPARAMS(_client, _commandArray[0]));
	else if (_ircServ.passwordIsValid(_commandArray[1]))
		return _client.setRegistered(SERV_REGISTRATION);
	else
		ft_send(_client, ERR_PASSWDMISMATCH);
}
