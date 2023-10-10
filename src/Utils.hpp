/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 22:51:50 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/10 22:54:19 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sstream>
#include "server/Server.hpp"

std::vector<std::string>	ft_split(std::string string, char separator);

std::string intToString(int number);

int	ft_send(Client user, std::string reply);

bool	checkPortNumber(char *portNumber);
bool	invalidInput(int argc, char **argv);