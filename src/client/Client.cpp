/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:43:08 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/05 19:13:29 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/******************************************************************************
* ------------------------------- CONSTRUCTORS ------------------------------ *
******************************************************************************/
Client::Client()
{
}

Client::~Client()
{
}

Client::Client(Client const &src){*this = src;}

Client &Client::operator=(Client const &src)
{
    if (this != &src)
    {
        this->_nickname = src._nickname;
        this->_username = src._username;
        this->_realname = src._realname;
        this->buffer = src.buffer;
        this->_registered = src._registered;
        this->_clientFd = src._clientFd;
        this->_clientIp = src._clientIp;
        this->_clientAddr = src._clientAddr;
        this->_clientPollFd = src._clientPollFd;
    }
    return *this;
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

std::string &Client::getBuffer()
{
    return this->buffer;
}
std::string Client::getHostName()
{
    std::string hostName = getNickname() + "!" + getUsername();
    return hostName;
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

void Client::setBuffer(std::string buffer)
{
    this->buffer += buffer;
}

/*****************************************************************************
* ---------------------------------- CHANNELS ------------------------------ *
*****************************************************************************/



/*****************************************************************************
* --------------------------- MEMBER FUNCTIOS ------------------------------ *
*****************************************************************************/

void Client::clearBuffer()
{
    this->buffer.clear();
}