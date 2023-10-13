/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 22:51:39 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 10:56:30 by moboigui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"
#include <limits>

int	ft_send(Client user, std::string reply) {
	std::cout << "-> " << reply << std::endl;
	reply.append("\n");
	if (send(user.getSocket(), reply.c_str(), reply.length(), 0) == -1) {
		std::cerr << "Failed to send message: " << reply << std::endl;
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

std::string intToString(int number)
{
	std::ostringstream oss;
	oss << number;
	std::string myString = oss.str();
	return myString;
}

int stringToInt(const std::string &str)
{
	long long result = 0;
	bool isNegative = false;

	for (size_t i = 0; i < str.length(); ++i)
	{
		if (i == 0 && str.at(i) == '-')
			isNegative = true;
		else if (str.at(i) >= '0' && str.at(i) <= '9')
			result = result * 10 + (str.at(i) - '0');
		else
			return -1;
		if (result > std::numeric_limits<int>::max() || result < std::numeric_limits<int>::min())
			return -1;
	}
	if (isNegative)
		result = -result;
	return result;
}
