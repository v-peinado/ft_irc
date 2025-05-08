/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:57:46 by vpeinado          #+#    #+#             */
/*   Updated: 2025/05/08 22:25:47 by vpeinado         ###   ########.fr       */
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
    if (this->_server.getUserByFd(fdClient)->getNickname() == "" 
        || this->_server.getUserByFd(fdClient)->getUsername() == "" 
        || this->_server.getUserByFd(fdClient)->getRealname() == "")
    {
        std::string channelName = ""; 
        std::string nickName = "";
        this->_server.sendError(451, nickName, channelName, fdClient, " :You have not registered\r\n");
        return 0;
    }
    std::string channelName = args.size() > 1 ? args[1] : "";     //args[1] channel name
    if (args.size() < 2 || args.size() > 3 || args[1].empty())
    {
        //<client> <command> :Not enough parameters
        this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
        return 0;
    }
    return 1;
}

void Join::sendJoinMessages(int fdClient, const std::string& channelName, Channel* channel) {
    // Get the client who has joined
    Client* client = this->_server.getUserByFd(fdClient);

    // JOIN message that will be sent to everyone in the channel
    std::string joinMsg = ":" + client->getHostName() + "@" + client->getClientIp() +
                            " JOIN " + channelName + "\r\n";
    channel->sendToAll(joinMsg); // Send the message to everyone in the channel

    // Response with the list of users in the channel (message 353)
    std::string rply = ": 353 " + client->getNickname() + " @ " + channelName + 
                        " :" + channel->ClientChannelList() + "\r\n";
    this->_server.sendResponse(rply, fdClient);

    // End of names list message (message 366)
    rply = ": 366 " + client->getNickname() + " " + channelName + " :End of /NAMES list\r\n";
    this->_server.sendResponse(rply, fdClient);

    // Send the channel topic (message 332)
    std::string topic = channel->GetTopicName().empty() ? "No topic is set" : channel->GetTopicName();
    rply = ": 332 " + client->getNickname() + " " + channelName + " :" + topic + "\r\n";
    this->_server.sendResponse(rply, fdClient);
}

void Join::run(std::vector<std::string> args, int fdClient)
{
    if (this->validArgs(args, fdClient) == 0)
        return;
    std::string channelName = args[1][0] == '#' ? args[1] : "#" + args[1]; // only add # if it doesn't have it, irssi does it automatically
    std::string channelKey = args.size() == 3 ? args[2] : "";              // maybe the ternary conditional is not allowed
    std::map<std::string, Channel *> &channels = this->_server.getChannels();
    std::map<std::string, Channel *>::iterator it = channels.find(channelName);
    if (it == channels.end()) // if the channel doesn't exist, create it
    {
        // Create new channel and set its attributes
        Channel *newChannel = new Channel();
        newChannel->SetName(channelName);
        newChannel->SetKey(channelKey);

        // If the channel key is empty, set it to 0, otherwise to 1
        if (channelKey == "")
            newChannel->SetHasKey(0);
        else
            newChannel->SetHasKey(1);
        channels.insert(std::pair<std::string, Channel *>(channelName, newChannel));
        newChannel->addClient(this->_server.getUserByFd(fdClient));
        newChannel->addAdmin(this->_server.getUserByFd(fdClient));
        // Inform everyone in the channel
        this->sendJoinMessages(fdClient, channelName, newChannel);
    }
    if (it != channels.end()) // not reaching the end of the map, so the channel already exists
    {
        // Check if the user is already in the channel
        if (it->second->GetClient(fdClient) != NULL)
        {
            std::cout << this->_server.getUserByFd(fdClient)->getNickname() << " is already on channel " << channelName << std::endl;
            // this->_server.sendError(443, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :is already on channel\r\n");
            return;
        }
        // Check if the channel has a key and if the key is correct
        if (it->second->GetHasKey() == 1 && it->second->GetKey() != channelKey)
        {
            this->_server.sendError(475, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Cannot join channel (+k) - bad key\r\n");
            return;
        }

        // Check if the channel is full
        if (it->second->GetLimit() != 0 && it->second->GetLimit() == it->second->GetClients().size())
        {
            this->_server.sendError(471, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Cannot join channel (+l)\r\n");
            return;
        }
        // Check if the channel is invite-only
        if (it->second->GetInviteOnly() == 1 && !it->second->isClientInvited(fdClient))
        {
            this->_server.sendError(473, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Cannot join channel (+i)\r\n");
            return;
        }
        // If invited, add to the channel
        it->second->addClient(this->_server.getUserByFd(fdClient));
        std::cout << this->_server.getUserByFd(fdClient)->getNickname() << " has joined the channel " << channelName << std::endl;

        // Inform everyone in the channel
        this->sendJoinMessages(fdClient, channelName, it->second);
    }
}
