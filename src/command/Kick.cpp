/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:31:07 by vpeinado          #+#    #+#             */
/*   Updated: 2025/05/08 22:27:17 by vpeinado         ###   ########.fr       */
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
        return;  // Validate the arguments

    std::string channelName = args[1];                      // Channel name
    std::string nicknameList = args[2];                     // List of nicknames to kick (possibly multiple, separated by commas)
    std::string reason = args.size() >= 4 ? args[3] : "";   // Reason for the kick (if exists)
    // If there's more than one word in the reason, concatenate them
    for(size_t i = 4; i < args.size(); i++) 
        reason += " " + args[i];

    // Validate if the channel exists
    if (!this->_server.getChannelByName(channelName)) 
    {
        this->_server.sendError(403, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :No such channel\r\n");
        return;
    }
    // Validate if the user executing the kick is in the channel
    if (!this->_server.getChannelByName(channelName)->isClientInChannel(fdClient)) 
    {
        this->_server.sendError(442, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :You're not on that channel\r\n");
        return;
    }
    // Validate if the user has channel operator permissions
    if (!this->_server.getChannelByName(channelName)->isClientAdmin(fdClient)) 
    {
        this->_server.sendError(482, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :You're not channel operator\r\n");
        return;
    }
    // Split the nickname list by commas
    std::vector<std::string> nicknames = splitNicknames(nicknameList, ',');

    // Iterate over each nickname to apply the kick logic
    for (size_t i = 0; i < nicknames.size(); ++i) 
    {
        std::string nickname = nicknames[i];

        // Validate if the user is trying to kick themselves
        if (this->_server.getUserByFd(fdClient)->getNickname() == nickname)   
            continue;       // Can't kick yourself

        // Validate if the user to kick is in the channel
        int targetClientFd = this->_server.getChannelByName(channelName)->GetClientFd(nickname);
        if (!this->_server.getChannelByName(channelName)->isClientInChannel(targetClientFd)) 
        {
            this->_server.sendError(441, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :They aren't on that channel\r\n");
            continue;
        }

        // Prepare the KICK message for all users in the channel and send it
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

        // Kick the user from the channel
        this->_server.getChannelByName(channelName)->removeClient(targetClientFd);

        // If the user is in the invited list, remove them from the list
        if (this->_server.getChannelByName(channelName)->isClientInvited(targetClientFd)) {
            this->_server.getChannelByName(channelName)->removeInvitedClient(targetClientFd);
        }
    }
}

