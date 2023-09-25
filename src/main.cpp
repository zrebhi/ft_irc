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

int main()
{
	if (fork() == 0)
	{
		Server ircServ;
	}
	else
	{
		sleep(2);
		virtualClient();
	}
	return 0;
}
