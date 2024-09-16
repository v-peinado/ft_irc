/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:53:24 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/16 12:11:35 by vpeinado         ###   ########.fr       */
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
    // Cambiaremos seguramente, por una funcion que no recibe un string, 
    // sino que setea a la hora actual del sistema
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

void Server::setPollfds(pollfd pollfd)
{
    this->_pollfds.push_back(pollfd);
}


/******************************************************************************
* ------------------------------- SERVER  ----------------------------------- *
******************************************************************************/

void Server::startServer(char *port, char *password)
{
    this->setPort(atoi(port)); //quiza deberiamos cambiar atoi por otro metodo
    this->setPassword(password);
    this->setActive(true);
    
    // Creacion del socket
    this->setServerFd(socket(AF_INET, SOCK_STREAM, 0));
    if (this->getServerFd() < 0)
    {
        // quiza conviene crear excepciones para los errores, y decidir si se cierra el programa o no
        std::cerr << "Error: socket" << std::endl;
        exit(1);
    }
    
    // Configuracion del serverAddr
    std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
    this->_serverAddr.sin_family = AF_INET; // ipv4
    this->_serverAddr.sin_addr.s_addr = INADDR_ANY; // Cualquier direccion puede conectarse
    this->_serverAddr.sin_port = htons(this->getPort()); // Puerto
    
    //!!!!!!IMPORTANTE!!!!!!
    //Configurar el socket para que pueda reutilizarse setsockopt, y fnctl para que no bloquee el puerto
    
    // Bind del socket(vincular)
    if(bind(this->getServerFd(), (struct sockaddr *)&this->getServerAddr(), sizeof(this->getServerAddr())) < 0)
    {
        // cambiar por excepciones
        std::cerr << "Error: bind" << std::endl;
        close(this->getServerFd());
        exit(1);
    }
    
    // Listen del socket
    if(listen(this->getServerFd(), MAX_CLIENTS) < 0)
    {
        std::cerr << "Error: listen" << std::endl;
        close(this->getServerFd());
        exit(1);
    }
    // Poll, añadir el socket del server a la lista de pollfds, tambien clientes, canales, etc
    // Despues de tener el vector de pollfds, se llama a runServer()
    // runServer() es el bucle principal del servidor, que constamente esta monitoreando los eventos de los sockets
    pollfd serverPollFd;
    serverPollFd.fd = this->getServerFd();
    serverPollFd.events = POLLIN;
    serverPollFd.revents = 0;
    this->setPollfds(serverPollFd);
}

void Server::runServer()
{
    /*
        Bucle principal del servidor, bucle infinito(mientras el servidor este activo)
        Poll, para monitorear los eventos de los sockets, con la lista de pollfds, que esta monitoreando
        Si hay un evento en el socket del servidor, como si alguien se conecta, se envia mensajes, etc
    */
   while(this->_active)
   {
        if (poll(this->_pollfds.data(), this->_pollfds.size(), -1) < 0)
        {
            // cambiar por excepciones
            std::cerr << "Error: poll" << std::endl;
            close(this->getServerFd());
            exit(1);
        }
        for (std::vector<pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); ++it)
        {
            if (it->revents & POLLIN) // Accedemos a los miembros con '->' porque 'it' es un puntero
            {
                if (it->fd == this->_serverFd)
                    this->newClientConnection();
                else
                    this->reciveNewData(it->fd);
            }
        }
   }
   // Cerrar el servidor, clientes, canales, etc
   // stopServer();
}
void Server::newClientConnection()
{
    // aceptar la conexion del cliente
}

void Server::reciveNewData(int fd)
{
    // Recibir datos del cliente
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