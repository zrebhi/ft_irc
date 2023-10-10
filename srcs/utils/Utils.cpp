/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 22:51:39 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/10 22:54:19 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

int	ft_send(Client user, std::string reply) {
	std::cout << "-> " << reply << std::endl;
	reply.append("\n");
	if (send(user.getSocket(), reply.c_str(), reply.length(), 0) == -1) {
		std::cerr << "Failed to send JOIN message" << std::endl;
		return 1;
	}
	return 0;
}

std::vector <std::string> ft_split(std::string string, char separator) {
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

std::string intToString(int number) {
	std::ostringstream oss;
	oss << number;
	std::string myString = oss.str();
	return myString;
}
