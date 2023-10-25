/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerReplies.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zrebhi <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 23:19:53 by zrebhi            #+#    #+#             */
/*   Updated: 2023/10/23 20:16:39 by zrebhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

#define RPL_INVITE_RCV(client, channelName, targetName) ":" + client.getNickname() + " INVITE " + targetName + " " + channelName
#define RPL_JOIN(nickname, channelName) ":" + nickname + " JOIN :#" + channelName
#define RPL_PRIVMSG(client, targetName, message) ":" + client.getNickname() + "!" + client.getUsername() + "@IRC PRIVMSG " + targetName + message
#define RPL_KICK(senderNick, channelName, targetNick, reason) ":" + senderNick + " KICK #" + channelName + " " + targetNick + " " + reason

/* 001 */ #define RPL_WELCOME(client) ":IRC 001 " + client.getNickname() + " :Welcome to the IRC network, " + client.getNickname() + "!"

/* 302 */ #define NICK(oldNickname, newNickname) ":" + oldNickname + " NICK :" + newNickname
/* 315 */ #define RPL_ENDOFWHO(client) ":IRC 315 " + client.getNickname() + " :End of WHO list"
/* 324 */ #define RPL_CHANNELMODEIS(client, channel) ":IRC 324 " + client.getNickname() + " #" + channel.getName() + " +n"
/* 331 */ #define RPL_NOTOPIC(channelName) ":IRC 331 " + channelName + " :No topic is set"
/* 332 */ #define RPL_TOPIC(client, channelName, topicText) ":IRC 332 " + client.getNickname() + " #" + channelName + " " + topicText
/* 341 */ #define RPL_INVITING(channelName, targetName) ":IRC 341 #" + channelName + " " + targetName
/* 353 */ #define RPL_NAMERPLY(client, channel) ":IRC 353 " + client.getNickname() + " = #" + channel.getName() + " :" + channel.userListString()
/* 366 */ #define RPL_ENDOFNAMES(channelName) ":IRC 366 " + channelName /* + " :End of NAMES list" */
/* 401 */ #define ERR_NOSUCHNICK(targetName) ":IRC 401 " + targetName + " :No such nick/channel"
/* 401 */ #define ERR_NOSUCHNICKCHAN(client, channelName, targetName, command) ":" + client.getNickname() + " PRIVMSG #" + channelName + " :Error " + command + ": No such nick (" + targetName + ")"
/* 403 */ #define ERR_NOSUCHCHANNEL(client, channelName) ":IRC 403 " + client.getNickname() + " " + channelName + " :No such channel"
/* 403 */ #define ERR_INVALIDCHANNEL(client, channelName) ":IRC 403 " + client.getNickname() + " " + channelName + " :Invalid channel name"
/* 416 */ #define ERR_TOOMANYMATCHES(client) ":IRC 416 " + client.getNickname() + " :Command or message too long"
/* 421 */ #define ERR_UNKNOWNCOMMAND(command) ":IRC 421 " + command + " :Unknown command"
/* 432 */ #define ERR_ERRONEUSNICKNAME(nickname) ":IRC 432 Error " + nickname + " :Erroneous nickname"
/* 433 */ #define ERR_NICKNAMEINUSE(clientName) ":IRC 433 * " + clientName + " :Nickname is already in use"
/* 441 */ #define ERR_USERNOTINCHANNEL(client, targetName, channelName) ":IRC 441 " + client.getNickname() + " #" + channelName + " " + targetName + " :They aren't on that channel"
/* 442 */ #define ERR_NOTONCHANNEL(channelName) channelName + " :You're not on that channel"
/* 443 */ #define ERR_USERONCHANNEL(targetName, channelName) ":IRC 443 " + targetName + " #" + channelName + " :is already on channel"
/* 451 */ #define ERR_NOTREGISTERED(client) ":IRC 451 " + client.getNickname() + " :You have not registered"
/* 461 */ #define ERR_NEEDMOREPARAMS(client, command) ":IRC 461 " + client.getNickname() + " " + command + " :Not enough parameters"
/* 462 */ #define ERR_TOOMANYPARAMS(command) ":IRC 462 " + command + " :Too many parameters"
/* 462 */ #define ERR_ALREADYREGISTRED(clientName) ":IRC 462 " + clientName + " :Already registered"
/* 464 */ #define ERR_PASSWDMISMATCH ":IRC 464 * :Password mismatch"
/* 471 */ #define ERR_CHANNELISFULL(client, channelName) ":IRC 471 " + channelName + " :Channel is full (+l)."
/* 472 */ #define ERR_UNKNOWNMODE(char, channelName) ":IRC 472 '" + char + "'" + " :is unknown mode char to me for #" + channelName
/* 473 */ #define ERR_INVITEONLYCHAN(channelName) channelName + " :Cannot join channel (+i)"
/* 475 */ #define ERR_INCORRECTPASSWORD(client, channelName) ":IRC 475 " + client.getNickname() + " " + channelName + " :Incorrect password for " + channelName
/* 476 */ #define ERR_BADCHANNELMASK(channelName) ":IRC 476 #" + channelName + " :Bad Channel Mask"
/* 476 */ #define ERR_BADLIMIT(client, channelName, input) ":IRC 476 " + client.getNickname() + " #" + channelName + " :Bad limit input (" + input + ")."
/* 482 */ #define ERR_CHANOPRIVSNEEDED(channelName, client) ":IRC 482 #" + client.getNickname() + " #" + channelName + " :You're not channel operator"

