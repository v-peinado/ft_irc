/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:53:24 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/16 17:59:49 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cstdlib>
#include "Client.hpp"

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

std::string const &Server::getServerName() const
{
    return this->_serverName;
}


// Client *Server::getUserByFd(int fd)
// {
//     std::map<int, Client *>::iterator it = this->_users.find(fd);    
//     if (it != this->_users.end()) 
//         return it->second;  // Devuelve el puntero al usuario si existe
//     else 
//         return NULL;  // Devuelve null si no se encuentra
// }

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

void Server::setPollfds(pollfd pollfd)
{
    this->_pollfds.push_back(pollfd);
}


/******************************************************************************
* ------------------------------- SERVER  ----------------------------------- *
******************************************************************************/
void Server::printServerInfo()
{
    // Imprime la informacion del servidor
    std::cout << "Server name: " << this->_serverName << std::endl;
    std::cout << "Server password: " << this->_password << std::endl;
    std::cout << "Server port: " << this->_port << std::endl;
    // Va a imprimir 0.0.0.0, porque acepta cualquier direccion
    std::cout << "IP: " << inet_ntoa(this->_serverAddr.sin_addr) << std::endl;
    std::cout << "Server fd: " << this->_serverFd << std::endl;
    std::cout << "Server active: " << this->_active << std::endl;
    // std::cout << "Server users: " << this->_users.size() << std::endl;
}
void Server::startServer(char *port, char *password)
{
    this->_serverName = "IRCserv: ffons-ti & vpeinado";
    this->_port = atoi(port);
    this->_password = password;
    this->_active = true;
    
    // Creacion del socket
    this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_serverFd < 0)
    {
        // quiza conviene crear excepciones para los errores, y decidir si se cierra el programa o no
        std::cerr << "Error: socket" << std::endl;
        exit(1);
    }
    
    // Configuracion del serverAddr
    std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
    this->_serverAddr.sin_family = AF_INET; // ipv4
    this->_serverAddr.sin_addr.s_addr = INADDR_ANY; // Cualquier direccion puede conectarse
    this->_serverAddr.sin_port = htons(this->_port); // Puerto
    
    //!!!!!!IMPORTANTE!!!!!! (para que no de error de bind)
    //Configurar el socket para que pueda reutilizarse setsockopt, y fnctl para que no bloquee el puerto
    int opt = 1;
    if(setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        // cambiar por excepciones
        std::cerr << "Error: setsockopt" << std::endl;
        close(this->_serverFd);
        exit(1);
    }
    if(fcntl(this->_serverFd, F_SETFL, O_NONBLOCK) < 0)
    {
        // cambiar por excepciones
        std::cerr << "Error: fcntl" << std::endl;
        close(this->_serverFd);
        exit(1);
    }
    // Bind del socket(vincular), hay que gestionar las signals, paraque al cerrar el servidor, se libere el puerto, si no dara fallo
    if(bind(this->_serverFd, (struct sockaddr *)&this->_serverAddr, sizeof(this->_serverAddr)) < 0)
    {
        // cambiar por excepciones
        std::cerr << "Error: bind" << std::endl;
        close(this->_serverFd);
        exit(1);
    }
    
    // Listen del socket, se usa para que el servidor pueda aceptar conexiones
    if(listen(this->_serverFd, MAX_CLIENTS) < 0)
    {
        std::cerr << "Error: listen" << std::endl;
        close(this->_serverFd);
        exit(1);
    }
    // Poll, añadir el socket del server a la lista de pollfds, tambien clientes, canales, etc
    // Despues de tener el vector de pollfds, se llama a runServer()
    // runServer() es el bucle principal del servidor, que constamente esta monitoreando los eventos de los sockets
    pollfd serverPollFd;
    serverPollFd.fd = this->_serverFd;
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
            close(this->_serverFd);
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
    Client newClient;
    memset(&newClient.getClientAddr(), 0, sizeof(newClient.getClientAddr()));
    socklen_t addrLen = sizeof(newClient.getClientAddr());
    int newClientFd = accept(this->_serverFd, (struct sockaddr *)&newClient.getClientAddr(), &addrLen);
    if (newClientFd < 0)
    {
        // cambiar por excepciones
        std::cerr << "Error: accept" << std::endl;
        close(this->_serverFd);
        exit(1);
    }
    newClient.getClientPollFd().fd = newClientFd;
    newClient.getClientPollFd().events = POLLIN;
    newClient.getClientPollFd().revents = 0;
    newClient.setClientFd(newClientFd);
    newClient.setClientIp(inet_ntoa(newClient.getClientAddr().sin_addr));
    // Insertar el nuevo cliente en la lista de usuarios
    this->setPollfds(newClient.getClientPollFd());
    std::cout << "New client connected: " << newClient.getClientIp() << std::endl;
}

void Server::reciveNewData(int fd)
{
    
}

void Server::stopServer()
{
    // Parada del server
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