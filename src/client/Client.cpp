/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:43:08 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/14 18:07:07 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/******************************************************************************
* ------------------------------- CONSTRUCTORS ------------------------------ *
******************************************************************************/

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
    return this->_realame;
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

std::map<std::string, Channel *> const &Client::getChannels() const
{
    return this->_channels;
}

Channel *Client::getChannelByName(std::string const &name) const
{
    std::map<std::string, ACommand *>::iterator it = this->_commands.find(name); 
    if (it != this->_commands.end())
        return it->second;
    else
        return NULL;
}

int const &Client::getClientFd() const
{
    return this->_clientFd;
}

struct Client::sockaddr_in const &Client::getClientAddr() const
{
    return this->_clientAddr;
}





