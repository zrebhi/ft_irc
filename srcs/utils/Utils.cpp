/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 22:51:39 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/19 21:57:12 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

void	ft_send(Client &user, std::string reply) {
	if (reply.empty())
		return;
	std::cout << "-> " << reply << std::endl;
	if (reply.at(reply.length() - 1) != '\n')
		reply.append("\n");
	if (send(user.getSocket(), reply.c_str(), reply.length(), 0) == -1) {
		std::cerr << "Failed to send message" << std::endl;
		return;
	}
	return;
}

std::vector<std::string> ft_split(std::string string, char separator) {
	std::vector<std::string> stringsVector;
	std::string newString;

	if (string.empty())
		return stringsVector;
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
	if (commandLine.empty())
		return result;
	result.push_back(firstWord(commandLine));

	if (result[0].size() < commandLine.size()) {
		std::string restOfLine = commandLine.substr(firstWord(commandLine).size() + 1);
		result.push_back(restOfLine);
	}

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

bool isValidNumber(const std::string &nbStr)
{
	for (size_t i = 0; i < nbStr.length(); i++)
	{
		if (!isdigit(nbStr.at(i)))
			return false;
	}
	return true;
}
