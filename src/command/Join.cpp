/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:57:46 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/06 19:50:51 by vpeinado         ###   ########.fr       */
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
    std::string channelName = args.size() > 1 ? args[1] : "";     //args[1] es el nombre del canal
    if (args.size() < 2 || args.size() > 3 || args[1].empty())
    {
        //<client> <command> :Not enough parameters
        this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
        return 0;
    }
    //irssi anade el canal por defecto si no se pone el # delante
    // if (channelName[0] != '#')
    // {
    //     this->_server.sendError(403, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :No such channel\r\n");
    //     return 0;
    // }
    return 1;
}

void Join::run(std::vector<std::string> args, int fdClient)
{
        if (this->validArgs(args, fdClient) == 0)
            return;
        std::string channelName = args[1][0] == '#' ? args[1] : "#" + args[1];  //solo se añade el # si no lo tiene, irssi lo hace automaticamente
        std::string channelKey = args.size() == 3 ? args[2] : ""; //quiza no se permitA el condicional ternario
        std::map<std::string, Channel *> &channels = this->_server.getChannels();
        std::map<std::string, Channel *>::iterator it = channels.find(channelName);
        if (it == channels.end())
        {
            Channel *newChannel = new Channel();
            newChannel->SetName(channelName);
            newChannel->SetKey(channelKey);
            if (channelKey == "")
                newChannel->SetHasKey(0);
            else
                newChannel->SetHasKey(1);
            channels.insert(std::pair<std::string, Channel *>(channelName, newChannel));
            newChannel->addClient(this->_server.getUserByFd(fdClient));
            newChannel->addAdmin(this->_server.getUserByFd(fdClient));
            std::string joinMsg = ":" + this->_server.getUserByFd(fdClient)->getHostName() +
                          "@" + this->_server.getUserByFd(fdClient)->getClientIp() +
                          " JOIN " + channelName + "\r\n";   
            newChannel->sendToAll(joinMsg);
            std::cout << this->_server.getUserByFd(fdClient)->getNickname() << " has joined the channel " << channelName << std::endl;
        }
        if (it != channels.end())
        {
            // Verificar si el usuario ya está en el canal
            if (it->second->GetClient(fdClient) != NULL)
            {
                //this->_server.sendError(443, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :is already on channel\r\n");
                return;
            }
            // Verificar si el canal tiene clave y si la clave es correcta
            if (it->second->GetHasKey() == 1 && it->second->GetKey() != channelKey)
            {
                this->_server.sendError(475, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Cannot join channel (+k) - bad key\r\n");
                return;
            }

            // Verificar si el canal está lleno
            if (it->second->GetLimit() != 0 && it->second->GetLimit() == it->second->GetClients().size())
            {
                this->_server.sendError(475, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Cannot join channel (+l)\r\n");
                return;
            }

            // Verificar si el canal es solo para invitados
            if (it->second->GetInviteOnly() == 1 && !it->second->isClientInvited(fdClient))
            {
                this->_server.sendError(471, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Cannot join channel (+i)\r\n");
                return;
            }

            // Si es invitado, agregarlo al canal
            it->second->addClient(this->_server.getUserByFd(fdClient));
            std::cout << this->_server.getUserByFd(fdClient)->getNickname() << " has joined the channel " << channelName << std::endl;

            // Informar a todos en el canal
            std::string joinMsg = ":" + this->_server.getUserByFd(fdClient)->getHostName() +
                          "@" + this->_server.getUserByFd(fdClient)->getClientIp() +
                          " JOIN " + channelName + "\r\n";   
            it->second->sendToAll(joinMsg); // Enviar el mensaje a todos en el canal
        }
            
}
