/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:39:42 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/03 23:29:13 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"

/*****************************************************************************
 * ------------------------------- CONSTRUCTORS ---------------------------- *
 ****************************************************************************/

Nick::Nick(Server &server) : ACommand(server)
{
    // Constructor
}

Nick::~Nick()
{
    // Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

int Nick::validArgs(std::vector<std::string> args, int fdClient)
{
    if (args.size() != 2)
    {
        send(fdClient, "461 NICK :Not enough parameters\r\n", 34, 0);
        return 0;
    }
    if (args[1].empty())
    {
        send(fdClient, "431 NICK :No nickname given\r\n", 30, 0);
        return 0;
    }
    // if (this->_server.getUserByFd(fdClient) == NULL || this->_server.getUserByFd(fdClient)->getRegistered() == 0
    //     || this->_server.getUserByFd(fdClient)->getRealname().empty() || this->_server.getUserByFd(fdClient)->getUsername().empty())
    // {
    //     send(fdClient, "451 NICK :You have not registered\r\n", 36, 0);
    //     return 0;
    // }
    for (size_t i = 0; i < args[1].length(); i++)
    {
        if (args[1][i] == ' ')
        {
            send(fdClient, "432 NICK :Erroneus nickname\r\n", 30, 0);
            return 0;
        }
    }
    std::map<int, Client *> users = this->_server.getUsers();
    for (std::map<int, Client *>::iterator it = users.begin(); it != users.end(); it++)       //comprobar si el nickname ya esta en uso
    {
        if (it->second->getNickname() == args[1])
        {
            send(fdClient, "433 NICK :Nickname is already in use\r\n", 39, 0);
            return 0;
        }
    }
    return 1;
}

void Nick::run(std::vector<std::string> args, int fdClient)
{
    if (validArgs(args, fdClient))
    {
        if (this->_server.getUserByFd(fdClient)->getRegistered() == 0)
        {
            send(fdClient, "451 NICK :You have not registered\r\n", 36, 0);
            return;
        }
        this->_server.getUserByFd(fdClient)->setNickname(args[1]);
    }
    else
    {
        return;
    }
}