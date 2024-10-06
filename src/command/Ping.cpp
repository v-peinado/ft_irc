/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:51:16 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/06 19:51:40 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Ping.hpp"

/*****************************************************************************
 * ------------------------------- CONSTRUCTORS ---------------------------- *
 ****************************************************************************/

Ping::Ping(Server &server) : ACommand(server)
{
    // Constructor
}

Ping::~Ping()
{
    // Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

int Ping::validArgs(std::vector<std::string> args)
{
    if (args.size() != 2)
        return (0);
    return (1);
}

void Ping::run(std::vector<std::string> args, int fdClient)
{
    if (this->validArgs(args) == 0)
    {
        std::string channelName = "";
        this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
        return;
    }
    std::cout << "PONG " + args[1] << std::endl;
    std::string response = ":localhost PONG " + args[1] + "\r\n";
    send(fdClient, response.c_str(), response.length(), 0);
}