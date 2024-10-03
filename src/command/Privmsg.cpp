/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:00:50 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/03 22:41:44 by vpeinado         ###   ########.fr       */
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
    if (args.size() < 3)
    {
        send(fdClient, "411 PRIVMSG :No recipient given\r\n", 34, 0);
        return 0;
    }
    if (args[2] == "")
    {
        send(fdClient, "412 PRIVMSG :No text to send\r\n", 30, 0);
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
    return targets;
}

void Privmsg::run(std::vector<std::string> args, int fdClient)
{
    if (this->validArgs(args, fdClient) == 0)
        return;

    // Separa los destinatarios y el mensaje
    std::vector<std::string> msgTargets = splitTarget(args[1]);
    std::string message = args[2];
    for (size_t i = 3; i < args.size(); i++)
        message += " " + args[i];

    std::map<int, Client *> users = this->_server.getUsers();
    std::map<std::string, Channel *> channels = this->_server.getChannels();

    for (size_t i = 0; i < msgTargets.size(); i++)
    {
        if (msgTargets[i][0] == '#') // Si el destino es un canal
        {
            Channel *channel = channels[msgTargets[i]];
            std::string user = this->_server.getUserByFd(fdClient)->getNickname();

            // Comprobar si el usuario está en el canal
            if (!userInChannel(channel, user))
            {
                send(fdClient, "404 PRIVMSG :Cannot send to channel\r\n", 38, 0);
                return;
            }

            // Verificar si el canal existe
            if (channel == NULL)
            {
                send(fdClient, "401 PRIVMSG :No such nick/channel\r\n", 34, 0);
                return;
            }

            // Enviar mensaje a todos los clientes en el canal
            for (size_t j = 0; j < channel->GetClients().size(); j++)
            {
                Client *recipient = channel->GetClients()[j];

                // No enviar al remitente
                if (recipient->getNickname() == user)
                    continue;  

                // Preparar el mensaje
                std::string rply = ":" + this->_server.getUserByFd(fdClient)->getNickname() + " PRIVMSG " + msgTargets[i] + " :" + message + "\r\n";

                // Enviar el mensaje
                send(recipient->getClientFd(), rply.c_str(), rply.size(), 0);
            }
        }
        else // Si el destino es un usuario
        {
            for (std::map<int, Client *>::iterator it = users.begin(); it != users.end(); it++)
            {
                if (it->first == fdClient) // No enviar al mismo cliente
                    continue;

                // Verificar si el usuario existe
                if (it->second->getNickname() == msgTargets[i])
                {
                    std::string rply = ":" + this->_server.getUserByFd(fdClient)->getNickname() + " PRIVMSG " + msgTargets[i] + " :" + message + "\r\n";
                    send(it->first, rply.c_str(), rply.size(), 0);
                }
                else
                {
                    send(fdClient, "401 PRIVMSG :No such nick/channel\r\n", 34, 0);
                    return;
                }
            }
        }
    }
}
