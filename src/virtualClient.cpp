#include "../inc/CommonLibs.hpp"

void virtualClient()
{
	int socketClient = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addrClient;
	addrClient.sin_addr.s_addr = INADDR_ANY;
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(8080);
	if (connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient)) < 0)
	{
		std::cerr << "Error at connection" << std::endl;
		exit(1);
	}
	else
	{
		std::cout << "Connected" << std::endl;
	}
	char buffer[1000];
	memset(buffer, 0, sizeof(buffer));
	recv(socketClient, buffer, sizeof(buffer), 0);
	std::cout << "server send: " << buffer << std::endl;
	for (int i = 0; i < 1000; i++)
	{
		sleep(100);
	}
	std::string str = "Hello how are you";
	send(socketClient, str.c_str(), str.size(), 0);
	str = "stop";
	send(socketClient, str.c_str(), str.size(), 0);
	close(socketClient);
}
