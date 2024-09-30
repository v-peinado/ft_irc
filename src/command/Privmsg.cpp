/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:00:50 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/30 11:29:58 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Privmsg.hpp"

/*****************************************************************************
 * ------------------------------- CONSTRUCTORS ---------------------------- *
 ****************************************************************************/

Privmsg::Privmsg(Server &server) : ACommand(server)
{
    // Constructor
}

Privmsg::~Privmsg()
{
    // Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

int Privmsg::validArgs(std::vector<std::string> args, int fdClient)
{
    if (args.size() < 3)
    {
        send(fdClient, "411 PRIVMSG :No recipient given\r\n", 34, 0);
        return 0;
    }
    if (args[2].empty())
    {
        send(fdClient, "412 PRIVMSG :No text to send\r\n", 30, 0);
        return 0;
    }
    return 1;
}

void Privmsg::run(std::vector<std::string> args, int fdClient)
{
    if (this->validArgs(args, fdClient) == 0)
        return;
    std::string message = args[2];
    for (size_t i = 3; i < args.size(); i++)
        message += " " + args[i];
    std::map<int, Client *> users = this->_server.getUsers();
    std::map<std::string, Channel *> channels = this->_server.getChannels();
    for (std::map<int, Client *>::iterator it = users.begin(); it != users.end(); it++)
    {
        if (it->first == fdClient)
            continue;
        if (it->second->getNickname() == args[1])
        {
            std::string rply = ":" + this->_server.getUserByFd(fdClient)->getNickname() + " PRIVMSG " + args[1] + " :" + message + "\r\n";
            send(it->first, rply.c_str(), rply.size(), 0);
            return;
        }
    }
    for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (it->first == args[1])
        {
            Channel *channel = it->second;
            for (size_t i = 0; i < channel->GetClients().size(); i++)
            {
                if (channel->GetClients()[i]->getNickname() == this->_server.getUserByFd(fdClient)->getNickname())
                    continue;
                std::string rply = ":" + this->_server.getUserByFd(fdClient)->getNickname() + " PRIVMSG " + args[1] + " :" + message + "\r\n";
                send(channel->GetClients()[i]->getClientFd(), rply.c_str(), rply.size(), 0);
            }
            return;
        }
    }
    send(fdClient, "401 PRIVMSG :No such nick/channel\r\n", 34, 0);
}