/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:19:13 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/07 13:22:38 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Pass.hpp"

/******************************************************************************
* ------------------------------- CONSTRUCTORS ------------------------------ *
******************************************************************************/

Pass::Pass(Server &server) : ACommand(server)
{
    // Constructor
}

Pass::~Pass()
{
    // Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

void Pass::run(std::vector<std::string> args, int fdClient)
{
    std::string nickName = "*";
    std::string channelName = "";
    if (args.size() != 2)
    {
        this->_server.sendError(461, nickName, channelName, fdClient, " :Not enough parameters\r\n");
        return;
    }
    if (this->_server.getUserByFd(fdClient)->getRegistered())
    {
        this->_server.sendError(462, nickName, channelName, fdClient, " :You may not reregister\r\n");
        return;
    }
    if (this->_server.getPassword() != args[1])
    {
        this->_server.sendError(464, nickName, channelName, fdClient, " :Password incorrect\r\n");
        return;
    }
    else
    {
        this->_server.getUserByFd(fdClient)->setRegistered(true);
    }
}