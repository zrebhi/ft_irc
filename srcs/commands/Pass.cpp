#include "Command.hpp"

void Command::pass()
{
	//mettre isProtected en premier pour qu'il ne calcule pas la commande
	//si le serveur n'est pas protege par un mot de passe
	if (!_ircServ.isProtected()) 
	{
		_client.setRegistered(SERV_REGISTRATION);
		std::string clientName = _client.getNickname();
		if (clientName.empty())
			clientName = "guest";
		return ft_send(_client, ERR_ALREADYREGISTRED(clientName));
	}
	if (_client.isRegistered())
		return ft_send(_client, ERR_ALREADYREGISTRED(_client.getNickname()));
	if (_commandArray.size() < 2 || _commandArray[1].empty())
		return ft_send(_client, ERR_NEEDMOREPARAMS(_client, _commandArray[0]));
	else if (_ircServ.passwordIsValid(_commandArray[1]))
		return _client.setRegistered(SERV_REGISTRATION);
	else
		ft_send(_client, ERR_PASSWDMISMATCH);
}
