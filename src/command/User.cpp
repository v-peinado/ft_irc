/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:08:07 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/03 23:37:14 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

/******************************************************************************
* ------------------------------- CONSTRUCTORS ------------------------------ *
******************************************************************************/

User::User(Server &server) : ACommand(server)
{
    // Constructor
}

User::~User()
{
    // Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

int User::validArgs(std::vector<std::string> args)
{
    if (args[1].empty() || args[2].empty() || args[3].empty() || args[4].empty())
        return 0;
    // if (args[2] != "0" || args[3] != "*")
    //     return 0;
    std::string realname = args[4];
    if (realname[0] != ':')
        return 0;
    return 1;
}

void User::run(std::vector<std::string> args, int fdClient)
{
    if (args.size() < 5)
    {
        send(fdClient, "461 USER :Not enough parameters\r\n", 35, 0);
        return;
    }
    if (this->_server.getUserByFd(fdClient)->getRegistered() == 0 || this->_server.getUserByFd(fdClient) == NULL || this->_server.getUserByFd(fdClient)->getNickname().empty())
    {
        send(fdClient, "451 USER :You have not registered\r\n", 36, 0);
        return;
    }
    if (validArgs(args))
    {
        this->_server.getUserByFd(fdClient)->setUsername(args[1]);
        this->_server.getUserByFd(fdClient)->setRealname(args[4]);
    }
    else
    {
        send(fdClient, "461 USER :Not enough parameters\r\n", 35, 0);
    }
    if (!this->_server.getUserByFd(fdClient)->getUsername().empty() && !this->_server.getUserByFd(fdClient)->getRealname().empty() && this->_server.getUserByFd(fdClient)->getRegistered() == 1)
    {
        std::string welcome = "001 " + args[1] + " :Welcome to the Internet Relay Network " + args[1] + "!" + this->_server.getUserByFd(fdClient)->getUsername() + "@" + this->_server.getServerName() + "\r\n";
        send(fdClient, welcome.c_str(), welcome.length(), 0);
    }
}
    