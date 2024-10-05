/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:51:16 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/05 21:35:20 by vpeinado         ###   ########.fr       */
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
        std::string response = fdClient + "461 PING :Not enough parameters\r\n";
        send(fdClient, response.c_str(), response.length(), 0);
        return;
    }
    std::cout << "PONG " + args[1] << std::endl;
    std::string response = "PONG " + args[1] + "\r\n";
    send(fdClient, response.c_str(), response.length(), 0);
}