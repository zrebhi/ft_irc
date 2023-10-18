/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerReplies.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 23:19:53 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/18 23:23:42 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

#define RPL_JOIN(client, channelName) ":" + client.getNickname() + " JOIN :#" + channelName

/* 001 */ #define RPL_WELCOME(client) ":IRC 001 " + client.getNickname() + " :Welcome to the IRC network, " + client.getNickname() + "!"

/* 302 */ #define NICK(oldNickname, newNickname) ":" + oldNickname + " NICK :" + newNickname

/* 353 */ #define RPL_NAMERPLY(client, channel) ":IRC 353 " + client.getNickname() + " = #" + channel.getName() + " :" + channel.userListString()

/* 403 */ #define ERR_NOSUCHCHANNEL(client, channelName) ":IRC 403 " + client.getNickname() + " " + channelName + " :No such channel"
/* 403 */ #define ERR_INVALIDCHANNEL(client, channelName) ":IRC 403 " + client.getNickname() + " " + channelName + " :Invalid channel name"
/* 432 */ #define ERR_ERRONEUSNICKNAME(nickname) ":IRC 432 * " + nickname + " :Erroneous nickname"
/* 433 */ #define ERR_NICKNAMEINUSE(client) ":IRC 433 * " + client.getNickname() + " :Nickname is already in use"
/* 461 */ #define ERR_NEEDMOREPARAMS(client, command) ":IRC 461 " + client.getNickname() + " " + command + " :Not enough parameters"
/* 462 */ #define ERR_TOOMANYPARAMS(command) ":IRC 462 " + command + " :Too many parameters"
/* 464 */ #define ERR_PASSWDMISMATCH ":IRC 464 * :Password mismatch"
/* 475 */ #define ERR_INCORRECTPASSWORD(client, channelName) ":IRC 475 " + client.getNickname() + " " + channelName + " :Incorrect password for " + channelName
/* 472 */ #define ERR_UNKNOWNMODE(char) ":IRC 472 '" + char + "'" + " :is unknown mode char to me"
/* 471 */ #define ERR_CHANNELISFULL(client, channelName) ":IRC 471 " + channelName + " :Channel is full (+l)."
/* 482 */ #define ERR_CHANOPRIVSNEEDED(channelName) channelName + " :You're not channel operator"
/* 442 */ #define ERR_NOTONCHANNEL(channelName) channelName + " :You're not on that channel"
/* 473 */ #define ERR_INVITEONLYCHAN(channelName) channelName + " :Cannot join channel (+i)"
