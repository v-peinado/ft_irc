/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:31:07 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/16 13:27:27 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Kick.hpp"

/*****************************************************************************
 * ------------------------------- CONSTRUCTORS ---------------------------- *
 ****************************************************************************/

Kick::Kick(Server &server) : ACommand(server)
{
    // Constructor
}

Kick::~Kick()
{
    // Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

int Kick::validArgs(std::vector<std::string> args, int fdClient)
{
    std::string channelName = "";
    if (args.size() < 3)
    {
        this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
        //send(fdClient, "461 KICK :Not enough parameters\r\n", 34, 0);
        return 0;
    }
    if (args[1].empty())
    {
        this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
        return 0;
    }
    if (args[2].empty())
    {
        this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
        return 0;
    }
    return 1;
}

void Kick::run(std::vector<std::string> args, int fdClient)
{
    if (this->validArgs(args, fdClient) == 0)                // Validar los argumentos
        return;
    //validar si existe el canal si el que esxpulsa es operator del canal y si el expulsado existe en el canal
    std::string channelName = args[1];                       // Nombre del canal
    std::string nickname = args[2];                           // Nickname del usuario a expulsar
    std::string reason = args.size() == 4 ? args[3] : "";     // Razon de la expulsión, si exsite una razon se van sumando als palabras
    for(size_t i = 3 ; i < args.size(); i++)
    {
        if(i == 3)
            reason = args[i];
        else
            reason += " " + args[i];
    }                                        
    //kick <#channel> <nickname> [:<reason>]
    //validar si se tienen permisos para hacer kick
    if (!this->_server.getChannelByName(channelName))
    {
        this->_server.sendError(403, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :No such channel\r\n");
        return;
    }
    if (!this->_server.getChannelByName(channelName)->isClientInChannel(fdClient))
    {
        this->_server.sendError(442, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :You're not on that channel\r\n");
        return;
    }
    if (!this->_server.getChannelByName(channelName)->isClientAdmin(fdClient))
    {
        this->_server.sendError(482, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :You're not channel operator\r\n");
        return;
    }
    if (this->_server.getUserByFd(fdClient)->getNickname() == nickname)
    {
        //send(fdClient, "482 KICK :You may not kick yourself\r\n", 39, 0);
        return;
    }
    if (!this->_server.getChannelByName(channelName)->isClientInChannel(this->_server.getChannelByName(channelName)->GetClientFd(nickname)))
    {
        this->_server.sendError(441, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :They aren't on that channel\r\n");
        return;
    }
    //avisar al canal que se ha expulsado a un usuario
    if (!reason.empty() && reason[0] != ':')
        reason = ":" + reason;
    if (!reason.empty())
        reason = " " + reason;
        
    std::string kickMsg = ":" + this->_server.getUserByFd(fdClient)->getHostName() +
        "@" + this->_server.getUserByFd(fdClient)->getClientIp() +
        " KICK " + channelName + " " + nickname + reason + "\r\n";  
    this->_server.getChannelByName(channelName)->sendToAll(kickMsg);
    //expulsar al usuario del canal 
    this->_server.getChannelByName(channelName)->removeClient(this->_server.getChannelByName(channelName)->GetClientFd(nickname));
    //borrarlo de la lista de invitados
    if (this->_server.getChannelByName(channelName)->isClientInvited(this->_server.getChannelByName(channelName)->GetClientFd(nickname)))
        this->_server.getChannelByName(channelName)->removeInvitedClient(this->_server.getChannelByName(channelName)->GetClientFd(nickname));   
}