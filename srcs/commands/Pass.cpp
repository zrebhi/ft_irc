#include "Command.hpp"

void Command::pass()
{
	if (this->_commandArray.size() < 2 || this->_commandArray[1].length() == 0)
		ft_send(this->_client, ERR_NEEDMOREPARAMS(this->_client, this->_commandArray[0]));
	else
		this->_client.setPassword(this->_commandArray[1]);
}

