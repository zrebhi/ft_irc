#include "../inc/CommonLibs.hpp"
#include <iostream>
#include <string>

void virtualClient(char letter)
{
	std::string msgList[] = {"hey salut", "ca va ?", "je suis le client A", "bonjour toi", "oui et toi ?", "et moi je suis le client B"};
	std::string color = letter == 'A' ? GREEN : YELLOW;
	// std::cout << color << "Enter virtual client " << letter << " " << RESET << std::endl;
	int socketClient = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addrClient;
	addrClient.sin_addr.s_addr = INADDR_ANY;
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(8080);
	if (connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient)) < 0)
	{
		std::cout << color << socketClient << letter << " Error at connection" << RESET << std::endl;
		exit(1);
	}
	else
		std::cout << color << socketClient << letter << " Connected" << RESET << std::endl;
	char buffer[1000];
	memset(buffer, 0, sizeof(buffer));
	std::string changeNick = "NICK ";
	recv(socketClient, buffer, sizeof(buffer), 0);
	std::cout << color << socketClient << letter << " recv: " << buffer << RESET << std::endl;
	while (changeNick.size() < 8)
	{
		changeNick.push_back(letter);
		send(socketClient, changeNick.c_str(), changeNick.size(), 0);
		memset(buffer, 0, sizeof(buffer));
		sleep(1);
		recv(socketClient, buffer, sizeof(buffer), 0);
		std::cout << color << socketClient << letter << " recv: " << buffer << RESET << std::endl;
	}
	sleep(2);
	for (int i = 0; i < 3; i++)
	{
		std::string sendMessage = letter == 'A' ? "PRIVMSG BBB " : "PRIVMSG AAA ";
		sendMessage += msgList[i + (letter == 'B' ? 3 : 0)];
		send(socketClient, sendMessage.c_str(), sendMessage.size(), 0);
		memset(buffer, 0, sizeof(buffer));
		recv(socketClient, buffer, sizeof(buffer), 0);
		std::cout << color << socketClient << letter << " recv: " << buffer << RESET << std::endl;
	}
	std::string joinMsg = "JOIN testChannel";
	send(socketClient, joinMsg.c_str(), joinMsg.size(), 0);
	recv(socketClient, buffer, sizeof(buffer), 0);
	std::cout << color << socketClient << letter << " recv: " << buffer << RESET << std::endl;
	sleep(10);
	std::string stop = "stop";
	if (letter == 'A')
		send(socketClient, stop.c_str(), stop.size(), 0);
	sleep(5);

	close(socketClient);
}
