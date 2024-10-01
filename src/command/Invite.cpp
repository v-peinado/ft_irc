/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:01:35 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/01 13:14:34 by vpeinado         ###   ########.fr       */
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
    if (args.size() < 3)
    {
        send(fdClient, "461 INVITE :Not enough parameters\r\n", 36, 0);
        return 0;
    }
    if (args[1].empty() || args[2].empty())
    {
        send(fdClient, "461 INVITE :Not enough parameters\r\n", 36, 0);
        return 0;
    }
    return 1;
}

void Invite::run(std::vector<std::string> args, int fdClient)
{
    std::string channelName = args[1];                       // Nombre del canal
    std::string nickname = args[2];                           // Nickname del usuario a invitar
    Channel *channel = this->_server.getChannelByName(channelName); // Buscar el canal por nombre
    if (channel == NULL)                                     // Si el canal no existe
    {
        send(fdClient, "403 INVITE :No such channel\r\n", 30, 0); // Enviar mensaje de error
        return;
    }
    Client *client = this->_server.getUserByFd(fdClient);    // Buscar el usuario por fd
    if (client == NULL)                                      // Si el usuario no existe
    {
        send(fdClient, "403 INVITE :No such nick/channel\r\n", 35, 0); // Enviar mensaje de error
        return;
    }
    if (channel->GetAdmin(client->getClientFd()) == NULL)     // Si el usuario no es admin del canal
    {
        send(fdClient, "482 INVITE :You're not channel operator\r\n", 45, 0); // Enviar mensaje de error
        return;
    }
    Client *invitedClient = this->_server.getUserByNick(nickname); // Buscar el usuario por nickname
    if (invitedClient == NULL)                                  // Si el usuario no existe
    {
        send(fdClient, "401 INVITE :No such nick/channel\r\n", 35, 0); // Enviar mensaje de error
        return;
    }
    if (channel->isClientInChannel(invitedClient->getClientFd())) // Si el usuario ya esta en el canal
    {
        send(fdClient, "443 INVITE :is already on channel\r\n", 35, 0); // Enviar mensaje de error
        return;
    }
    if (channel->isClientInvited(invitedClient->getClientFd())) // Si el usuario ya ha sido invitado
    {
        send(fdClient, "443 INVITE :is already invited\r\n", 35, 0); // Enviar mensaje de error
        return;
    }
    channel->addClient(invitedClient); // Añadir el usuario al canal
    send(fdClient, "341 INVITE :Invite sent\r\n", 30, 0); // Enviar mensaje de exito
    send(invitedClient->getClientFd(), "341 INVITE :Invite received\r\n", 34, 0); // Enviar mensaje al usuario invitado
}