/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:00:50 by vpeinado          #+#    #+#             */
/*   Updated: 2025/05/08 22:30:23 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Privmsg.hpp"

/*****************************************************************************
 * ------------------------------- CONSTRUCTORS ---------------------------- *
 ****************************************************************************/

Privmsg::Privmsg(Server &server) : ACommand(server)
{
    // Constructor
}

Privmsg::~Privmsg()
{
    // Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

int Privmsg::validArgs(std::vector<std::string> args, int fdClient)
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
        this->_server.sendError(411, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :No recipient given\r\n");
        return 0;
    }
    if (args[2] == "" || args[2][0] != ':')
    {
        this->_server.sendError(412, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :No text to send\r\n");
        return 0;
    }
    return 1;
}

bool  Privmsg::userInChannel(Channel *chan, std::string user)
{
    if (chan == NULL)
        return false;
    for (size_t i = 0; i < chan->GetClients().size(); i++)
    {
        if (chan->GetClients()[i]->getNickname() == user)
            return true;
    }
    return false;
}
std::vector<std::string> Privmsg::splitTarget(std::string str)
{
    std::vector<std::string> targets;
    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos)
    {
        token = str.substr(0, pos);
        targets.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    targets.push_back(str);
    for (size_t i = 0; i < targets.size(); i++)
    {
        std::cout << "Target: " << targets[i] << std::endl;
    }
    return targets;
}

void Privmsg::run(std::vector<std::string> args, int fdClient)
{
    if (this->validArgs(args, fdClient) == 0)
        return;

    // Separate recipients and message
    std::vector<std::string> msgTargets = splitTarget(args[1]);
    std::string message = args[2];
    for (size_t i = 3; i < args.size(); i++)
        message += " " + args[i];

    std::map<int, Client *> users = this->_server.getUsers();
    std::map<std::string, Channel *> channels = this->_server.getChannels();

    for (size_t i = 0; i < msgTargets.size(); i++)
    {
        if (msgTargets[i][0] == '#') // If the target is a channel
        {
            Channel *channel = channels[msgTargets[i]];
            std::string user = this->_server.getUserByFd(fdClient)->getNickname();

            // Check if the channel exists
            if (this->_server.channelExist(msgTargets[i]) == false)
            {
                this->_server.sendError(401, this->_server.getUserByFd(fdClient)->getNickname(), msgTargets[i], fdClient, " :No such nick/channel\r\n");
                return;
            }
            
            // Check if the user is in the channel
            if (!userInChannel(channel, user))
            {
                this->_server.sendError(404, this->_server.getUserByFd(fdClient)->getNickname(), msgTargets[i], fdClient, " :Cannot send to channel\r\n");
                return;
            }

            // Send message to all clients in the channel
            for (size_t j = 0; j < channel->GetClients().size(); j++)
            {
                Client *recipient = channel->GetClients()[j];

                // Don't send to the sender
                if (recipient->getNickname() == user)
                    continue;  

                // Prepare the message
                std::string rply = ":" + this->_server.getUserByFd(fdClient)->getHostName() + "@" + this->_server.getUserByFd(fdClient)->getClientIp()
                                + " PRIVMSG " + msgTargets[i] + " " + message + "\r\n";

                // Send the message
                send(recipient->getClientFd(), rply.c_str(), rply.size(), 0);
            }
        }
        else // If the target is a user
        {
            if (this->_server.ifClientExist(msgTargets[i]))
            {
                Client *recipient = this->_server.getUserByNick(msgTargets[i]);
                std::string user = this->_server.getUserByFd(fdClient)->getNickname();

                // Prepare the message
                std::string rply = ":" + this->_server.getUserByFd(fdClient)->getHostName() 
                        + "@" + this->_server.getUserByFd(fdClient)->getClientIp() + " PRIVMSG " + msgTargets[i] + " " + message + "\r\n";

                // Send the message
                send(recipient->getClientFd(), rply.c_str(), rply.size(), 0);
            }
            else
            {
                this->_server.sendError(401, this->_server.getUserByFd(fdClient)->getNickname(), msgTargets[i], fdClient, ": No such nick/channel\r\n");
                return;
            }
        }
    }
}