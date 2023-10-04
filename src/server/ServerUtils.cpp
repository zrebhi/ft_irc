/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 20:59:05 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/04 23:05:20 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::map<std::string, Channel> Server::getChannelList() const {
	return this->_channels;
}
std::map<int, Client>	Server::getClientList() const {
	return this->_clientSockets;
}

std::vector <std::string> Server::ft_split(std::string string, char separator) {
	std::vector<std::string> stringsVector;
	std::string newString;

	for (size_t i = 0; i < string.length(); i++) {
		if (string[i] == separator) {
			stringsVector.push_back(newString);
			newString.clear();
		}
		else if (i == string.length() - 1) {
			stringsVector.push_back(newString);
			newString.clear();
		}
		else
			newString += string[i];
	}
	return stringsVector;
}

int	ft_send(Client user, std::string reply) {
	std::cout << "-> " << reply << std::endl;
	reply.append("\n");
	if (send(user.getSocket(), reply.c_str(), reply.length(), 0) == -1) {
		std::cerr << "Failed to send JOIN message" << std::endl;
		return 1;
	}
	return 0;
}
