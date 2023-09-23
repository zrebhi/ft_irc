#include <arpa/inet.h>
#include <cstdlib>
#include <iostream>
#include <netinet/in.h>
#include <ostream>
#include <sys/socket.h>
#include <unistd.h>

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
	recv(socketClient, buffer, sizeof(buffer), 0);
	std::cout << "server send: " << buffer << std::endl;
	close(socketClient);
}
