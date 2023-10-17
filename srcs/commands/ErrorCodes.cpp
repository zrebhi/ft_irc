#include "../inc/ErrorCodes.hpp"

std::string ErrorCodes::sendErrMsg(int code) const
{
	if (errorMessages_.find(code) != errorMessages_.end())
	{
		std::stringstream message;
		message << ":" << _host << " " << code << " " << _nickname;
		if (_param.empty() == false)
			message << " " << _param;
		message << errorMessages_.at(code);
		send(_socket, message.str().c_str(), message.str().size(), 0);
		return message.str();
	}
	return "Code d'erreur inconnu";
}
