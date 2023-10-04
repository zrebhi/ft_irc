/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 23:24:53 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/04 22:42:15 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void Command::user() {
	this->_client.setUsername(this->_commandArray[1]);
	std::string reply = ":IRC 001 " + this->_client.getNickname() + " :Welcome to the Internet Relay Network " +
			this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + this->_client.getHostname();
	ft_send(this->_client, reply);
}

void Command::nick() {
	this->_client.setNickname(this->_commandArray[1]);
}
