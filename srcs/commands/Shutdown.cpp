/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shutdown.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:17:56 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/11 20:55:09 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::shutdown() {
	std::map<int, Client> clients = this->_ircServ.getClientList();
	std::map<int, Client>::iterator it = clients.begin();

	for (; it != clients.end(); ++it)
		close(it->first);
	close(this->_ircServ.getServerSocket());
	close(this->_ircServ.getEpollFd());
	this->_ircServ.serverShutdown();
}

