/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 18:52:11 by zrebhi            #+#    #+#             */
/*   Updated: 2023/09/21 19:44:26 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/CommonLibs.hpp"
#include "../inc/Server.hpp"
#include <iostream>

int main()
{
	std::cout << "Colors: \n"
			  << RED << "Server" << std::endl;
	std::cout << GREEN << "Client A\n"
			  << YELLOW << "Client B\n"
			  << RESET << std::endl;
	if (fork() == 0)
	{
		if (fork() == 0)
		{
			virtualClient('A');
		}
		else
		{
			sleep(3);
			virtualClient('B');
		}
	}
	else
	{
		Server ircServ;
	}
	return 0;
}
