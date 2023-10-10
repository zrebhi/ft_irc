/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 19:03:43 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/03 19:51:47 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Command::Command(const std::vector <std::string> &commandArray, Client &client, Server &ircServ) :
_commandArray(commandArray), _client(client) , _ircServ(ircServ) {}

Command::~Command() {}