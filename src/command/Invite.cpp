/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:01:35 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/18 12:08:44 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Invite.hpp"

/*****************************************************************************
 * ------------------------------- CONSTRUCTORS ---------------------------- *
 ****************************************************************************/

Invite::Invite(Server &server) : ACommand(server)
{
    // Constructor
}

Invite::~Invite()
{
    // Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

int Invite::validArgs(std::vector<std::string> args, int fdClient)
{
    if (this->_server.getUserByFd(fdClient)->getNickname() == "" 
        || this->_server.getUserByFd(fdClient)->getUsername() == "" 
        || this->_server.getUserByFd(fdClient)->getRealname() == "")
    {
        std::string channelName = ""; 
        std::string nickName = "";
        this->_server.sendError(451, nickName, channelName, fdClient, " :You have not registered\r\n");
        return 0;
    }
    std::string channelName = "";
    if (args.size() < 3)
    {
        this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
        return 0;
    }
    if (args[1].empty() || args[2].empty())
    {
        this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
        return 0;
    }
    return 1;
}

void Invite::run(std::vector<std::string> args, int fdClient)
{
    if (this->validArgs(args, fdClient) == 0)
        return;  // Validar los argumentos
    std::string channelName = args[2];                       // Nombre del canal
    std::string nickname = args[1];                           // Nickname del usuario a invitar
    Channel *channel = this->_server.getChannelByName(channelName); // Buscar el canal por nombre
    if (channel == NULL)                                     // Si el canal no existe
    {
        this->_server.sendError(403, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :No such channel\r\n");
        return;
    }
    Client *client = this->_server.getUserByFd(fdClient);    // Buscar el usuario por fd
    if (client == NULL)                                      // Si el usuario no existe
    {
        this->_server.sendError(401, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :No such nick/channel\r\n");
        return;
    }
    if (channel->GetAdmin(client->getClientFd()) == NULL)     // Si el usuario no es admin del canal
    {
        this->_server.sendError(482, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :You're not channel operator\r\n");
        return;
    }
    Client *invitedClient = this->_server.getUserByNick(nickname); // Buscar el usuario por nickname
    if (invitedClient == NULL)                                  // Si el usuario no existe
    {
        this->_server.sendError(401, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :No such nick/channel\r\n");
        return;
    }
    if (channel->isClientInChannel(invitedClient->getClientFd())) // Si el usuario ya esta en el canal
    {
        this->_server.sendError(443, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :is already on channel\r\n");
        return;
    }
    if (channel->isClientInvited(invitedClient->getClientFd())) // Si el usuario ya ha sido invitado
    {
        this->_server.sendError(443, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :is already on channel\r\n");
        return;
    }
    channel->addInvitedClients(invitedClient->getClientFd()); // Añadir el usuario al canal
    std::string inviteMsg = ": 341 " + this->_server.getUserByFd(fdClient)->getNickname() + " " + invitedClient->getNickname() + " " + channelName + "\r\n";
    send(fdClient, inviteMsg.c_str(), inviteMsg.size(), 0); // Enviar mensaje de exito
    std::string invite = ":" + invitedClient->getHostName() + 
                            " INVITE " + invitedClient->getNickname() + " " + channelName + "\r\n";
    send(invitedClient->getClientFd(), invite.c_str(), invite.size(), 0); // Enviar mensaje al usuario invitado
}