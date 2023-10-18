/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 22:51:39 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/18 23:41:48 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

int	ft_send(Client user, std::string reply) {
	std::cout << "-> " << reply << std::endl;
	if (reply.at(reply.length() - 1) != '\n')
		reply.append("\n");
	if (send(user.getSocket(), reply.c_str(), reply.length(), 0) == -1) {
		std::cerr << "Failed to send JOIN message" << std::endl;
		return 1;
	}
	return 0;
}

#include <cctype>  // Include for isprint function

//voir si il faut traiter tout ce qui vient apres un : comment une seule string
std::vector<std::string> ft_split(std::string string, char separator) {
	std::vector<std::string> stringsVector;
	std::string newString;

	for (size_t i = 0; i < string.length(); i++) {
		if (string[i] == separator) {
			stringsVector.push_back(newString);
			newString.clear();
		} else if (string[i] != '\r') {
			newString += string[i];
		}
	}
	// Add the last substring if it's not empty
	if (!newString.empty())
		stringsVector.push_back(newString);

	return stringsVector;
}

std::string intToString(int number) {
	std::ostringstream oss;
	oss << number;
	std::string myString = oss.str();
	return myString;
}

int stringToInt(const std::string& str) {
	std::stringstream ss(str);
	int result = 0;

	ss >> result;
	return result;
}

std::string firstWord(std::string commandLine) {
	std::istringstream iss(commandLine);
	std::string firstWord;
	iss >> firstWord;
	return firstWord;
}

std::vector<std::string> firstWordSplit(std::string &commandLine) {
	std::vector<std::string> result;
	result.push_back(firstWord(commandLine));

	std::string restOfLine = commandLine.substr(firstWord(commandLine).size() + 1);
	result.push_back(restOfLine);

	return result;
}

std::vector<std::string>	ircCommandSplitter(std::string command) {
	std::vector<std::string>	commandArray;

	if (firstWord(command) == "NICK" || firstWord(command) == "PASS" || \
	firstWord(command) == "USER")
		commandArray = firstWordSplit(command);
	else
		commandArray = ft_split(command, ' ');
	return commandArray;
}
