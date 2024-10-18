/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:31:07 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/18 17:37:51 by vpeinado         ###   ########.fr       */
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
    if (args.size() >= 4 && args[3][0] != ':')
    {
        this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
        return 0;
    }
    return 1;
}

// Función para dividir una cadena por un delimitador (como ',')
std::vector<std::string> splitNicknames(const std::string &nicknames, char delimiter) 
{
    std::vector<std::string> result;
    std::stringstream ss(nicknames);
    std::string item;
    
    while (std::getline(ss, item, delimiter)) 
        result.push_back(item);  
    return result;
}

void Kick::run(std::vector<std::string> args, int fdClient) 
{
    if (this->validArgs(args, fdClient) == 0)
        return;  // Validar los argumentos

    std::string channelName = args[1];                      // Nombre del canal
    std::string nicknameList = args[2];                     // Lista de nicknames a expulsar (posiblemente múltiples, separados por comas)
    std::string reason = args.size() >= 4 ? args[3] : "";   // Razón de la expulsión (si existe)
    // Si hay más de una palabra en la razón, se concatenan
    for(size_t i = 4; i < args.size(); i++) 
        reason += " " + args[i];

    // Validar si existe el canal
    if (!this->_server.getChannelByName(channelName)) 
    {
        this->_server.sendError(403, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :No such channel\r\n");
        return;
    }
    // Validar si el usuario que ejecuta el kick está en el canal
    if (!this->_server.getChannelByName(channelName)->isClientInChannel(fdClient)) 
    {
        this->_server.sendError(442, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :You're not on that channel\r\n");
        return;
    }
    // Validar si el usuario tiene permisos de operador del canal
    if (!this->_server.getChannelByName(channelName)->isClientAdmin(fdClient)) 
    {
        this->_server.sendError(482, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :You're not channel operator\r\n");
        return;
    }
    // Dividir la lista de nicknames por comas
    std::vector<std::string> nicknames = splitNicknames(nicknameList, ',');

    // Iterar sobre cada nickname para aplicar la lógica de expulsión
    for (size_t i = 0; i < nicknames.size(); ++i) 
    {
        std::string nickname = nicknames[i];

        // Validar si el usuario intenta expulsarse a sí mismo
        if (this->_server.getUserByFd(fdClient)->getNickname() == nickname)   
            continue;       // No se puede expulsar a sí mismo

        // Validar si el usuario a expulsar está en el canal
        int targetClientFd = this->_server.getChannelByName(channelName)->GetClientFd(nickname);
        if (!this->_server.getChannelByName(channelName)->isClientInChannel(targetClientFd)) 
        {
            this->_server.sendError(441, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :They aren't on that channel\r\n");
            continue;
        }

        // Preparar el mensaje de KICK para todos los usuarios en el canal y enviarlo
        if (reason.empty())
        {
            std::string kickMsg = ":" + this->_server.getUserByFd(fdClient)->getHostName() +
                              "@" + this->_server.getUserByFd(fdClient)->getClientIp() +
                              " KICK " + channelName + " " + nickname + "\r\n";
            this->_server.getChannelByName(channelName)->sendToAll(kickMsg);
        } 
        else
        {
            std::string kickMsg = ":" + this->_server.getUserByFd(fdClient)->getHostName() +
                              "@" + this->_server.getUserByFd(fdClient)->getClientIp() +
                              " KICK " + channelName + " " + nickname + " " + reason + "\r\n";
            this->_server.getChannelByName(channelName)->sendToAll(kickMsg);
        }

        // Expulsar al usuario del canal
        this->_server.getChannelByName(channelName)->removeClient(targetClientFd);

        // Si el usuario está en la lista de invitados, eliminarlo de la lista
        if (this->_server.getChannelByName(channelName)->isClientInvited(targetClientFd)) {
            this->_server.getChannelByName(channelName)->removeInvitedClient(targetClientFd);
        }
    }
}

