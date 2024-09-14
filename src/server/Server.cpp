/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:53:24 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/14 23:09:00 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cstdlib>

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

int Server::getPort() const
{
    return this->_port;
}

std::string const &Server::getServerName() const
{
    return this->_serverName;
}

std::string const &Server::getPassword() const
{
    return this->_password;
}

std::string const &Server::getWelcomeMessage() const
{
    return this->_welcomeMessage;
}

std::string const &Server::getTimeServer() const
{
    return this->_timeServer;
}

bool const &Server::getActive() const
{
    return this->_active;
}

int const &Server::getServerFd() const
{
    return this->_serverFd;
}

struct sockaddr_in const &Server::getServerAddr() const
{
    return this->_serverAddr;
}

Client *Server::getUserByFd(int fd)
{
    std::map<int, Client *>::iterator it = this->_users.find(fd);    
    if (it != this->_users.end()) 
        return it->second;  // Devuelve el puntero al usuario si existe
    else 
        return NULL;  // Devuelve null si no se encuentra
}

// Channel *Server::getChannelByName(std::string const &name)
// {
//     std::map<std::string, Channel *>::iterator it = this->_channels.find(name);
//     if (it != this->_channels.end())
//         return it->second;
//     else
//         return NULL;  
// }

// ACommand *Server::getCommandByName(std::string const &name)
// {
//     std::map<std::string, ACommand *>::iterator it = this->_commands.find(name);
//     if (it != this->_commands.end())
//         return it->second;
//     else
//         return NULL;
// }

std::map<int , Client *> const &Server::getUsers() const
{
    return this->_users;
}

// std::map<std::string , Channel *> const &Server::getChannels() const
// {
//     return this->_channels;
// }

// std::map<std::string , ACommand *> const &Server::getCommands() const
// {
//     return this->_commands;
// }

/******************************************************************************
* ------------------------------- SETTERS ----------------------------------- *
**************************************************************************** */

void Server::setPort(int port)
{
    this->_port = port;
}

void Server::setServerName(std::string const &serverName)
{
    this->_serverName = serverName;
}

void Server::setPassword(std::string const &password)
{
    this->_password = password;
}

void Server::setWelcomeMessage(std::string const &welcomeMessage)
{
    this->_welcomeMessage = welcomeMessage;
}

void Server::setTimeServer(std::string const &timeServer)
{
    this->_timeServer = timeServer;
}

void Server::setActive(bool active)
{
    this->_active = active;
}

void Server::setServerFd(int serverFd)
{
    this->_serverFd = serverFd;
}

void Server::setServerAddr(struct sockaddr_in serverAddr)
{
    this->_serverAddr = serverAddr;
}

/******************************************************************************
* ------------------------------- SERVER  ----------------------------------- *
******************************************************************************/

void Server::startServer(char *port, char *password)
{
    this->setPort(atoi(port)); //no se puede atoi en c++98
    this->setPassword(password);
    this->setActive(true);
    // Creacion del socket
    this->setServerFd(socket(AF_INET, SOCK_STREAM, 0));
    if (this->getServerFd() == -1)
    {
        std::cerr << "Error: socket" << std::endl;
        exit(1);
    }
    this->_serverAddr.sin_family = AF_INET; // ipv4
    this->_serverAddr.sin_addr.s_addr = INADDR_ANY; // Cualquier direccion puede conectarse
    this->_serverAddr.sin_port = htons(this->getPort()); // Puerto
    
    // Configuracion del socket
    // Bind del socket
    // Listen del socket
    // Accept del socket
    // Poll
    // Cierre del socket
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

void Server::insertUser(int fd, Client *user)
{
    // Inserta un usuario
}

// void Server::insertChannel(std::string const &name, Channel *channel)
// {
//     // Inserta un canal
// }

// void Server::insertCommand(std::string const &name, ACommand *command)
// {
//     // Inserta un comando
// }

void Server::deleteUser(int fd)
{
    // Elimina un usuario
}

void Server::deleteChannel(std::string const &name)
{
    // Elimina un canal
}

// Channel *Server::newChannel(std::string const &name)
// {
//     // Crea un nuevo canal
// }

/******************************************************************************
* --------------------------------- POLL  ----------------------------------- *
******************************************************************************/


// proxima implementacion