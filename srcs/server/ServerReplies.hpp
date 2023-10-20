/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerReplies.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 23:19:53 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/21 01:10:07 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

#define RPL_JOIN(nickname, channelName) ":" + nickname + " JOIN :#" + channelName
#define RPL_PRIVMSG(client, targetName, message) ":" + client.getNickname() + "!" + client.getUsername() + "@IRC PRIVMSG " + targetName + message
#define RPL_KICK(senderNick, channelName, targetNick, reason) ":" + senderNick + " KICK #" + channelName + " " + targetNick + " " + reason

/* 001 */ #define RPL_WELCOME(client) ":IRC 001 " + client.getNickname() + " :Welcome to the IRC network, " + client.getNickname() + "!"

/* 302 */ #define NICK(oldNickname, newNickname) ":" + oldNickname + " NICK :" + newNickname
/* 324 */ #define RPL_CHANNELMODEIS(client, channel) ":IRC 324 " + client.getNickname() + " #" + channel.getName() + " +n"
/* 332 */ #define RPL_TOPIC(client, channelName, topicText) ":IRC 332 " + client.getNickname() + " #" + channelName + " " + topicText
/* 341 */ #define RPL_INVITING(channelName, targetName) ":IRC 341 #" + channelName + " " + targetName
/* 353 */ #define RPL_NAMERPLY(client, channel) ":IRC 353 " + client.getNickname() + " = #" + channel.getName() + " :" + channel.userListString()

/* 401 */ #define ERR_NOSUCHNICK(targetName) ":IRC 401 " + targetName + " :No such nick/channel"
/* 403 */ #define ERR_NOSUCHCHANNEL(client, channelName) ":IRC 403 " + client.getNickname() + " " + channelName + " :No such channel"
/* 403 */ #define ERR_INVALIDCHANNEL(client, channelName) ":IRC 403 " + client.getNickname() + " " + channelName + " :Invalid channel name"
/* 432 */ #define ERR_ERRONEUSNICKNAME(nickname) ":IRC 432 * " + nickname + " :Erroneous nickname"
/* 433 */ #define ERR_NICKNAMEINUSE(client) ":IRC 433 * " + client.getNickname() + " :Nickname is already in use"
/* 441 */ #define ERR_USERNOTINCHANNEL(client, targetName, channelName) ":IRC 441 " + client.getNickname() + " #" + channelName + " " + targetName + " :They aren't on that channel"
/* 442 */ #define ERR_NOTONCHANNEL(channelName) channelName + " :You're not on that channel"
/* 443 */ #define ERR_USERONCHANNEL(targetName, channelName) ":IRC " + targetName + " #" + channelName + " :is already on channel"
/* 461 */ #define ERR_NEEDMOREPARAMS(client, command) ":IRC 461 " + client.getNickname() + " " + command + " :Not enough parameters"
/* 462 */ #define ERR_TOOMANYPARAMS(command) ":IRC 462 " + command + " :Too many parameters"
/* 464 */ #define ERR_PASSWDMISMATCH ":IRC 464 * :Password mismatch"
/* 471 */ #define ERR_CHANNELISFULL(client, channelName) ":IRC 471 " + channelName + " :Channel is full (+l)."
/* 472 */ #define ERR_UNKNOWNMODE(char) ":IRC 472 '" + char + "'" + " :is unknown mode char to me"
/* 473 */ #define ERR_INVITEONLYCHAN(channelName) channelName + " :Cannot join channel (+i)"
/* 475 */ #define ERR_INCORRECTPASSWORD(client, channelName) ":IRC 475 " + client.getNickname() + " " + channelName + " :Incorrect password for " + channelName
/* 476 */ #define ERR_BADCHANNELMASK(channelName) ":IRC 476 #" + channelName + " :Bad Channel Mask"
/* 482 */ #define ERR_CHANOPRIVSNEEDED(channelName, client) ":IRC 482 " + client.getNickname() + " #" + channelName + " :You're not channel operator"
