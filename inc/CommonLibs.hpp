#ifndef COMMONLIBS_HPP
#define COMMONLIBS_HPP

#include <arpa/inet.h>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <ostream>
#include <poll.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

// Codes d'échappement ANSI pour les couleurs
#define RESET "\033[0m"               // Réinitialisation des couleurs
#define BLACK "\033[30m"              // Noir
#define RED "\033[31m"                // Rouge
#define GREEN "\033[32m"              // Vert
#define YELLOW "\033[33m"             // Jaune
#define BLUE "\033[34m"               // Bleu
#define MAGENTA "\033[35m"            // Magenta
#define CYAN "\033[36m"               // Cyan
#define WHITE "\033[37m"              // Blanc
#define BOLDBLACK "\033[1m\033[30m"   // Noir gras
#define BOLDRED "\033[1m\033[31m"     // Rouge gras
#define BOLDGREEN "\033[1m\033[32m"   // Vert gras
#define BOLDYELLOW "\033[1m\033[33m"  // Jaune gras
#define BOLDBLUE "\033[1m\033[34m"    // Bleu gras
#define BOLDMAGENTA "\033[1m\033[35m" // Magenta gras
#define BOLDCYAN "\033[1m\033[36m"    // Cyan gras
#define BOLDWHITE "\033[1m\033[37m"   // Blanc gras

#define NOT_REGISTERED "undefined"
#define CMD_SIZE 5
#define EVENT_LIST_SIZE 11

class ErrorHandler : public std::exception
{
  public:
	ErrorHandler(int errorCode)
		: _errorCode(errorCode)
	{
	}

	int getCode() const
	{
		return _errorCode;
	}

  private:
	int _errorCode;
};

#endif // !COMMONLIBS_HPP
