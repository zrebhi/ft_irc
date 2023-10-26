/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:52:11 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/18 23:42:23 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"

int	main(int argc, char **argv) {
	if (invalidInput(argc, argv))
		return 1;

	std::string password;
	int	portNumber = stringToInt(argv[1]);
	if (argc == 3)
		password = argv[2];

	Server	ircServ(portNumber, password);

	while (ircServ.serverRunning())
		ircServ.listenToNewEvents();
	return 0;
}

bool	invalidInput(int argc, char **argv) {
	if (argc < 2 || argc > 3) {
		std::cerr << "usage: ./ft_irc <portNumber> <password>" << std::endl;
		return true;
	}
	if (!checkPortNumber(argv[1])) {
		std::cerr << "invalid port number" << std::endl;
		return true;
	}
	return false;
}

bool	checkPortNumber(char *portNumber) {
	for (int i = 0; portNumber[i]; i++) {
		if (!isdigit(portNumber[i]))
			return false;
		}
	return true;
}
