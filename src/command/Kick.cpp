/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:31:07 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/01 13:19:35 by vpeinado         ###   ########.fr       */
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
    if (args.size() < 3)
    {
        send(fdClient, "461 KICK :Not enough parameters\r\n", 34, 0);
        return 0;
    }
    if (args[1].empty())
    {
        send(fdClient, "461 KICK :Not enough parameters\r\n", 34, 0);
        return 0;
    }
    if (args[2].empty())
    {
        send(fdClient, "461 KICK :Not enough parameters\r\n", 34, 0);
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
        reason += " " + args[i];                                         
    //kick <#channel> <nickname> [:<reason>]
    //validar si se tienen permisos para hacer kick
    if (!this->_server.getChannelByName(channelName))
    {
        send(fdClient, "403 KICK :No such channel\r\n", 32, 0);
        return;
    }
    if (!this->_server.getChannelByName(channelName)->isClientInChannel(fdClient))
    {
        send(fdClient, "442 KICK :You're not on that channel\r\n", 39, 0);
        return;
    }
    if (!this->_server.getChannelByName(channelName)->isClientAdmin(fdClient))
    {
        send(fdClient, "482 KICK :You're not channel operator\r\n", 41, 0);
        return;
    }
    if (this->_server.getUserByFd(fdClient)->getNickname() == nickname)
    {
        send(fdClient, "482 KICK :You may not kick yourself\r\n", 39, 0);
        return;
    }
    
    if (!this->_server.getChannelByName(channelName)->isClientInChannel(this->_server.getChannelByName(channelName)->GetClientFd(nickname)))
    {
        send(fdClient, "441 KICK :They aren't on that channel\r\n", 41, 0);
        return;
    }
    if (this->_server.getChannelByName(channelName)->isClientAdmin(this->_server.getChannelByName(channelName)->GetClientFd(nickname))) // si un operador intenta echar a otro operador
    {
        send(fdClient, "482 KICK :You're not channel operator\r\n", 41, 0);
        return;
    }
    //expulsar al usuario del canal 
    this->_server.getChannelByName(channelName)->removeClient(this->_server.getChannelByName(channelName)->GetClientFd(nickname));
    //borrarlo de la lista de invitados
    if (this->_server.getChannelByName(channelName)->isClientInvited(this->_server.getChannelByName(channelName)->GetClientFd(nickname)))
        this->_server.getChannelByName(channelName)->removeInvitedClient(this->_server.getChannelByName(channelName)->GetClientFd(nickname));   
    //avisar al canal
    if (reason.empty())
        reason = "Kicked by " + this->_server.getUserByFd(fdClient)->getNickname();
    std::string msg = ":" + this->_server.getUserByFd(fdClient)->getNickname() + " KICK " + channelName + " " + nickname + " :" + reason + "\r\n";
    this->_server.getChannelByName(channelName)->sendToAll(msg);

    //avisar al expulsado
    send(this->_server.getChannelByName(channelName)->GetClientFd(nickname), msg.c_str(), msg.size(), 0);
}