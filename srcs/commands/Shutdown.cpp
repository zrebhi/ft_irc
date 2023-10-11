/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shutdown.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 18:17:56 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/11 18:17:56 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::shutdown() {
	std::vector<Client> clients = this->_ircServ.getClientList();
	std::vector<Client>::iterator it = clients.begin();

	for (; it != clients.end(); ++it)
		close(it->getSocket());
	close(this->_ircServ.getServerSocket());
	this->_ircServ.serverShutdown();
}