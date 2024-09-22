/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:43:08 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/22 17:07:16 by vpeinado         ###   ########.fr       */
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