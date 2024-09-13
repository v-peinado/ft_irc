/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:53:24 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/13 17:21:51 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/******************************************************************************
* ------------------------------- CONSTRUCTORS ------------------------------ *
******************************************************************************/

Server::Server()
{
    // Constructor
    // Inicializacion de variables, mejor por lista de inicializacion
}

Server::~Server()
{
    // Destructor
    // Limpieza de maps, liberacion de memoria
}


/*****************************************************************************
* ---------------------------------- GETTERS ------------------------------- *
*****************************************************************************/

Server::getPort() const
{
    return this->port;
}

std::string const &Server::getServerName() const
{
    return this->serverName;
}

std::string const &Server::getPassword() const
{
    return this->password;
}

std::string const &Server::getWelcomeMessage() const
{
    return this->welcomeMessage;
}

std::string const &Server::getTimeServer() const
{
    return this->timeServer;
}

bool const &Server::getActive() const
{
    return this->active;
}

int const &Server::getServerFd() const
{
    return this->serverFd;
}

struct sockaddr_in const &Server::getServerAddr() const
{
    return this->serverAddr;
}

AClient *Server::getUserByFd(int fd)
{
    return this->users[fd];
}

Channel *Server::getChannelByName(std::string const &name)
{
    return this->channels[name];
}

ACommand *Server::getCommandByName(std::string const &name)
{
    return this->commands[name];
}

std::map<int , AClient *> const &Server::getUsers() const
{
    return this->users;
}

std::map<string::string , Channel *> const &Server::getChannels() const
{
    return this->channels;
}

std::map<string::string , ACommand *> const &Server::getCommands() const
{
    return this->commands;
}

/******************************************************************************
* ------------------------------- SETTERS ----------------------------------- *
**************************************************************************** */

void Server::setPort(int port)
{
    this->port = port;
}

void Server::setServerName(std::string const &serverName)
{
    this->serverName = serverName;
}

void Server::setPassword(std::string const &password)
{
    this->password = password;
}

void Server::setWelcomeMessage(std::string const &welcomeMessage)
{
    this->welcomeMessage = welcomeMessage;
}

void Server::setTimeServer(std::string const &timeServer)
{
    this->timeServer = timeServer;
}

void Server::setActive(bool active)
{
    this->active = active;
}

void Server::setServerFd(int serverFd)
{
    this->serverFd = serverFd;
}

void Server::setServerAddr(struct sockaddr_in serverAddr)
{
    this->serverAddr = serverAddr;
}

/******************************************************************************
* ------------------------------- SERVER  ----------------------------------- *
******************************************************************************/

void Server::startServer()
{
    // Inicializacion del server
}

void Server::stopServer()
{
    // Parada del server
}

void Server::restartServer()
{
    // Reinicio del server
}

void Server::sendToClient(int fd, std::string const &message)
{
    // Envia un mensaje a un cliente
}

void Server::receiveFromClient(int fd, std::string const &message)
{
    // Recibe un mensaje de un cliente
}

void Server::connection()
{
    // Conexion de un cliente
}

void Server::insertUser(int fd, AClient *user)
{
    // Inserta un usuario
}

void Server::insertChannel(std::string const &name, Channel *channel)
{
    // Inserta un canal
}

void Server::insertCommand(std::string const &name, ACommand *command)
{
    // Inserta un comando
}

void Server::deleteUser(int fd)
{
    // Elimina un usuario
}

void Server::deleteChannel(std::string const &name)
{
    // Elimina un canal
}

Channel *Server::newChannel(std::string const &name)
{
    // Crea un nuevo canal
}

/******************************************************************************
* --------------------------------- POLL  ----------------------------------- *
******************************************************************************/


// proxima implementacion