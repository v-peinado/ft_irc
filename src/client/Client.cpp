/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:43:08 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/16 17:09:07 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/******************************************************************************
* ------------------------------- CONSTRUCTORS ------------------------------ *
******************************************************************************/
Client::Client()
{
    // Constructor
    // Inicializacion de variables, mejor por lista de inicializacion
}
Client::Client(struct sockaddr_in client, int fd)
{
    // Inicializar variables
    //mejor lsita de inicializacion
}

Client::~Client()
{
    // Destructor
    // Limpieza de maps, liberacion de memoria
}

/*****************************************************************************
* ---------------------------------- GETTERS ------------------------------- *
*****************************************************************************/

std::string const &Client::getNickname() const
{
    return this->_nickname;
}

std::string const &Client::getUsername() const
{
    return this->_username;
}

std::string const &Client::getRealname() const
{
    return this->_realname;
}

bool const &Client::getRegistered() const
{
    return this->_registered;
}

bool const &Client::getActive() const
{
    return this->_active;
}

int const &Client::getLevel() const
{
    return this->_level;
}

// std::map<std::string, Channel *> const &Client::getChannels() const
// {
//     return this->_channels;
// }

// Channel *Client::getChannelByName(std::string const &name) const
// {
//     std::map<std::string, ACommand *>::iterator it = this->_commands.find(name); 
//     if (it != this->_commands.end())
//         return it->second;
//     else
//         return NULL;
// }

int const &Client::getClientFd() const
{
    return this->_clientFd;
}

struct sockaddr_in &Client::getClientAddr()
{
    return this->_clientAddr;
}

struct pollfd &Client::getClientPollFd()
{
    return this->_clientPollFd;
}

std::string const &Client::getClientIp() const
{
    return this->_clientIp;
}



/*****************************************************************************
* ---------------------------------- SETTERS ------------------------------- *
*****************************************************************************/

void Client::setNickname(std::string const &nickname)
{
    this->_nickname = nickname;
}

void Client::setUsername(std::string const &username)
{
    this->_username = username;
}

void Client::setRealname(std::string const &realname)
{
    this->_realname = realname;
}

void Client::setRegistered(bool registered)
{
    this->_registered = registered;
}

void Client::setActive(bool active)
{
    this->_active = active;
}

void Client::setLevel(int level)
{
    this->_level = level;
}

void Client::setClientFd(int clientFd)
{
    this->_clientFd = clientFd;
}

void Client::setClientAddr(struct sockaddr_in clientAddr)
{
    this->_clientAddr = clientAddr;
}

void Client::setClientIp(std::string clientIp)
{
    this->_clientIp = clientIp;
}

/*****************************************************************************
* ---------------------------------- CHANNELS ------------------------------ *
*****************************************************************************/

// void Client::insertChannel(std::string const &name, Channel *channel)
// {
//     this->_channels.insert(std::pair<std::string, Channel *>(name, channel));
// }

// void Client::eraseChannel(std::string const &name)
// {
//     this->_channels.erase(name);
// }

// void Client::leaveChannel(std::string const &name)
// {
//     //De momento es lo mismo que eraseChannel, pero en un futuro se podria hacer algo mas
//     this->_channels.erase(name);
// }

// void Client::leaveAllChannels()
// {
//     //Liberar memoria de los canales, si se implementa,, modificacion en futuro segun necesidad
//     this->_channels.clear();
// }
