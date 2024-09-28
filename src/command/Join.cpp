/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:57:46 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/28 22:06:49 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"

/*****************************************************************************
 * ------------------------------- CONSTRUCTORS ---------------------------- *
 ****************************************************************************/

Join::Join(Server &server) : ACommand(server)
{
    // Constructor
}

Join::~Join()
{
    // Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

int Join::validArgs(std::vector<std::string> args, int fdClient)
{
    if (args.size() < 2 || args.size() > 3 || args[1].empty())
    {
        send(fdClient, "461 JOIN :Not enough parameters\r\n", 34, 0);
        return 0;
    }
    if (args[1][0] != '#')
    {
        send(fdClient, "403 JOIN :No such channel\r\n", 28, 0);
        return 0;
    }
    if (args[1].length() > 50)
    {
        send(fdClient, "405 JOIN :Channel name too long\r\n", 33, 0);
        return 0;
    }
    if (args.size() == 3 && args[2].length() > 50)
    {
        send(fdClient, "405 JOIN :Channel key too long\r\n", 32, 0);
        return 0;
    }
    return 1;
}

void Join::run(std::vector<std::string> args, int fdClient)
{
    if (this->validArgs(args, fdClient) == 0)
        return;
    std::string channelName = args[1];
    std::string channelKey = args.size() == 3 ? args[2] : ""; // quiza no se permitA el condicional ternario
    std::map<std::string, Channel *> &channels = this->_server.getChannels();
    std::map<std::string, Channel *>::iterator it = channels.find(channelName);
    if (it == channels.end())
    {
        Channel *newChannel = new Channel();
        newChannel->SetName(channelName);
        newChannel->SetKey(channelKey);
        channels.insert(std::pair<std::string, Channel *>(channelName, newChannel));
        newChannel->addClient(this->_server.getUserByFd(fdClient));
        newChannel->addAdmin(this->_server.getUserByFd(fdClient));
        it->second->sendToAll("JOIN :" + this->_server.getUserByFd(fdClient)->getNickname() + "\r\n");
        std::cout << this->_server.getUserByFd(fdClient)->getNickname() << " has joined the channel " << channelName << std::endl;
    }
    else
    {
        if (it->second->GetClient(fdClient) != NULL)
        {
            send(fdClient, "443 JOIN :Already in channel\r\n", 31, 0);
            return;
        }
        // check si el canal tiene clave y si la clave es correcta
        if (it->second->GetHasKey() == 1 && it->second->GetKey() != channelKey)
        {
            send(fdClient, "475 JOIN :Cannot join channel (+k)\r\n", 36, 0);
            return;
        }
        // check si el canal esta lleno
        if (it->second->GetLimit() != 0 && it->second->GetLimit() == it->second->GetClients().size())
        {
            send(fdClient, "471 JOIN :Cannot join channel (+l)\r\n", 36, 0);
            return;
        }
        // check si el canal es solo para invitados y si estas invitado
        if (it->second->GetInviteOnly() == 1 && !it->second->isClientInvited(fdClient))
        {
            send(fdClient, "473 JOIN :Cannot join channel (+i)\r\n", 36, 0);
            return;
        }
        if (it->second->GetInviteOnly() == 1 && it->second->isClientInvited(fdClient))
        {

            it->second->addClient(this->_server.getUserByFd(fdClient));
            std::cout << this->_server.getUserByFd(fdClient)->getNickname() << " has joined the channel " << channelName << std::endl;
            it->second->sendToAll("JOIN :" + this->_server.getUserByFd(fdClient)->getNickname() + "\r\n");
            return;
        }
        it->second->addClient(this->_server.getUserByFd(fdClient));
        std::cout << this->_server.getUserByFd(fdClient)->getNickname() << " has joined the channel " << channelName << std::endl;
        it->second->sendToAll("JOIN :" + this->_server.getUserByFd(fdClient)->getNickname() + "\r\n");
    }
}
