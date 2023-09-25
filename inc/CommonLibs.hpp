#ifndef COMMONLIBS_HPP
#define COMMONLIBS_HPP
#include <arpa/inet.h>
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/epoll.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define CMD_SIZE 5

typedef struct s_cmd
{
	std::string cmdType;
	std::vector<std::string> params;
} t_cmd;
#endif // !COMMONLIBS_HPP
