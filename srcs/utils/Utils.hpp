/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 22:51:50 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/13 20:42:22 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include <cctype>
#include "../server/Server.hpp"

#define ADD true
#define REMOVE false
#define NICK_REGISTRATION true
#define SERV_REGISTRATION false 
#define MAX_CHAN_USERS 100
#define MIN_CHAN_USERS 2


std::vector<std::string>	ft_split(std::string string, char separator);

std::string					firstWord(std::string commandLine);
std::vector<std::string>	firstWordSplit(std::string &commandLine);

std::vector<std::string>	ircCommandSplitter(std::string command);

std::string					intToString(int number);

int							ft_send(Client user, std::string reply);

bool						checkPortNumber(char *portNumber);
bool						invalidInput(int argc, char **argv);
