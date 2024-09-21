/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:53:24 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/21 15:19:37 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Pass.hpp"
#include "User.hpp"
#include "Nick.hpp"

/******************************************************************************
* ------------------------------- CONSTRUCTORS ------------------------------ *
******************************************************************************/

Server::Server(char *port, char *password)
{
    this->_serverName = "IRCserv: ffons-ti & vpeinado";
    this->_port = atoi(port);
    this->_password = password;
    this->_active = true;
    this->setWelcomeMessage();
}

Server::~Server(){}

/*****************************************************************************
* ---------------------------------- GETTERS ------------------------------- *
*****************************************************************************/

std::string const &Server::getServerName() const
{
    return this->_serverName;
}

std::string const &Server::getPassword() const
{
    return this->_password;
}
Client *Server::getUserByFd(int fd)
{
    std::map<int, Client *>::iterator it = this->_users.find(fd);
    if (it != this->_users.end()) 
        return it->second;
    else 
        return NULL;
}

std::map<int , Client *> const &Server::getUsers() const
{
    return this->_users;
}

/******************************************************************************
* ------------------------------- SETTERS ----------------------------------- *
**************************************************************************** */

void Server::setPollfds(pollfd pollfd)
{
    if (this->_pollfds.size() >= MAX_CLIENTS)                             // MAX_CLIENTS = 10
    {
        throw std::runtime_error("Error: max clients reached");
        return;
    }
    if (pollfd.fd < 3)                                                    // 0, 1, 2 son los fd reservados
    {
        throw std::runtime_error("Error: invalid fd");
        return;
    }
    this->_pollfds.push_back(pollfd);                                     // Añadir el pollfd al vector de pollfds
}

void Server::setWelcomeMessage()
{
    this->_welcomeMessage = 
        "*************************************************\n"
        "*          Welcome to the IRC Server!           *\n"
        "*   1. Use PASS <password> to log in            *\n"
        "*   2. Use USER <username> 0 * :<realname>      *\n"
        "*   3. Use NICK <nickname> to set your nickname *\n"
        "*************************************************\n";
}


/******************************************************************************
* ------------------------------- SERVER  ----------------------------------- *
******************************************************************************/
void Server::printServerInfo()
{
    std::cout << "╔════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║                🌐 IRC SERVER INFORMATION               ║" << std::endl;
    std::cout << "╠════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "  Server Name     : " << this->_serverName << std::endl;
    std::cout << "  Server Password : " << this->_password << std::endl;
    std::cout << "  Server Port     : " << this->_port << std::endl;
    std::cout << "  Server File Descriptor (fd) : " << this->_serverFd << std::endl;
    std::cout << "╠════════════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║                🚀 Server is up and running!            ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════╝" << std::endl;
    }
void Server::startServer()
{   
    setSocket();                  // Crear el socket

    configServerAddr();           // Configurar la direccion del servidor
    
    setSocketOptions();           // Opcion 1: Reusar la direccion del servidor

    bindSockets();                // Vincular el socket al puerto dado

    listenAndPoll();              // Escuchar y monitorear los eventos de los sockets
}

void Server::setSocket()
{
    this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);           // Crear el socket, AF_INET = IPv4, SOCK_STREAM = TCP, 0 = Protocolo por defecto
    if (this->_serverFd < 0)
    {
        throw std::runtime_error("Error: socket failed");
        exit(1);
    }
}

void Server::configServerAddr()                                      // sockaddr_in, se utiliza para definir una dirección de socket IPv4, incluyendo el puerto y la dirección IP.
{                                    
    std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));   // Limpiar la estructura de la direccion del servidor
    this->_serverAddr.sin_family = AF_INET;                          // Familia de direcciones, AF_INET = IPv4
    this->_serverAddr.sin_addr.s_addr = INADDR_ANY;                  // Recibir conexiones de cualquier direccion IP, 0.0.0.0               
    this->_serverAddr.sin_port = htons(this->_port);                 // Puerto del servidor, htons() convierte el entero corto sin signo del host al formato de red
}

void Server::setSocketOptions()
{
    int opt = 1;                                                                           // Opcion para reusar la direccion del servidor
    if(setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)       // SOL_SOCKET = Nivel de socket, SO_REUSEADDR = Reusar la direccion del servidor 
    {
        throw std::runtime_error("Error: setsockopt");
        close(this->_serverFd);
        exit(1);
    }
    if(fcntl(this->_serverFd, F_SETFL, O_NONBLOCK) < 0)                                     // Configura fd para ser no bloqueante, no esperarán a que se complete la operación si no hay datos disponibles
    {
        throw std::runtime_error("Error: fcntl");
        close(this->_serverFd);
        exit(1);
    }
}

void Server::bindSockets()
{
    if(bind(this->_serverFd, (struct sockaddr *)&this->_serverAddr, sizeof(this->_serverAddr)) < 0)     // Vincular el socket al puerto dado
    {
        throw std::runtime_error("Error: bind");
        close(this->_serverFd);
        exit(1);
    }
}

void Server::listenAndPoll()
{
    if(listen(this->_serverFd, MAX_CLIENTS) < 0)        //Escuchar las conexiones entrantes, es decir, esperar a que los clientes se conecten    
    {
        throw std::runtime_error("Error: listen");
        close(this->_serverFd);
        exit(1);
    }
    pollfd serverPollFd;                               // Estructura pollfd, monitorear los eventos de los sockets
    serverPollFd.fd = this->_serverFd;                 // File descriptor del servidor
    serverPollFd.events = POLLIN;                      // Eventos a monitorear, POLLIN = Datos listos para leer
    serverPollFd.revents = 0;                          // Eventos que ocurrieron, inicializado a 0
    this->setPollfds(serverPollFd);
}

void Server::runServer()
{
   while(this->_active)                                                     // Bucle principal del servidor, se ejecuta mientras el servidor este activo
   {                                                                            
        if (poll(this->_pollfds.data(), this->_pollfds.size(), -1) < 0)     // poll() monitorea los eventos de los sockets, -1 = Esperar indefinidamente, se llama en cada iteracion del bucle
        {                                                                   // monitoreando los cambios en los descriptores de archivo cada iteracion
            throw std::runtime_error("Error: poll");
            close(this->_serverFd);
            exit(1);
        }

        for (size_t i = 0; i < this->_pollfds.size(); i++)                  // Itera a través de todos los elementos del vector de pollfds
        {
            if (this->_pollfds[i].revents & POLLIN)                         // Verifica si hay eventos de lectura disponibles en el socket actual
            {
                if (this->_pollfds[i].fd == this->_serverFd)                // Comprueba si el socket activo es el socket del servidor, significa que se intenta acceder a el
                    this->newClientConnection();                            // Si es el socket del servidor, llama a la función para aceptar nuevas conexiones de clientes
                else
                    this->reciveNewData(this->_pollfds[i].fd);              // Si el socket activo no es el  servidor, significa que se reciben datos de un cliente ya conectado
            }
        }
   }
   // Cerrar el servidor, clientes, canales, etc
   // stopServer();
}

void Server::stopServer()
{
    // Parada del server
}

/******************************************************************************
* ------------------------------- CLIENTS ----------------------------------- *
******************************************************************************/

void Server::newClientConnection()
{
    // Crear un nuevo cliente dinámicamente en el heap
    Client *newClient = new Client();
    memset(&newClient->getClientAddr(), 0, sizeof(newClient->getClientAddr()));
    socklen_t addrLen = sizeof(newClient->getClientAddr());

    // Aceptar la conexión
    int newClientFd = accept(this->_serverFd, (struct sockaddr *)&newClient->getClientAddr(), &addrLen);
    //std::cout << "New client fd: " << newClientFd << std::endl;
    if (newClientFd < 0)
    {
        throw std::runtime_error("Error: accept");
        delete newClient; // Asegurarse de liberar la memoria en caso de error
        return;
    }
    //std::cout << "New client fd: " << newClientFd << std::endl;
    // Configurar el nuevo cliente
    newClient->getClientPollFd().fd = newClientFd;
    newClient->getClientPollFd().events = POLLIN;
    newClient->getClientPollFd().revents = 0;
    newClient->setClientFd(newClientFd);
    newClient->setClientIp(inet_ntoa(newClient->getClientAddr().sin_addr));
    newClient->setRegistered(false);

    // Añadir el nuevo cliente al vector de pollfds
    this->setPollfds(newClient->getClientPollFd());

    // Insertar el nuevo cliente en el mapa _users
    this->_users.insert(std::make_pair(newClientFd, newClient));

    // Mensaje de conexión del cliente
    std::cout << "New client connected: " << newClient->getClientIp() << std::endl;

    // Enviar el mensaje de bienvenida al cliente
    send(newClientFd, this->_welcomeMessage.c_str(), this->_welcomeMessage.size(), 0);
}

void Server::reciveNewData(int fd)
{
    std::vector<std::string> data; // usaremos el vector para guardar los posibles comandos almacenados en un solo mensaje
    char buffer[1024]; // Buffer para recibir los datos, es neceseario porque recv no puede recibir un string directamente
    memset(buffer, 0, 1024);
    Client *client = this->_users[fd]; // Representa al cliente que envia los datos, que esta en la lista de usuarios
    int bytes = recv(fd, buffer, 1024, 0);
    if (bytes < 0)
    {    
         throw std::runtime_error("Error: recv");
    }
    else if (bytes == 0)
    {
        // Cliente desconectado
        std::cout << "Client disconnected, fd: " << fd << std::endl;
        this->deleteClientPollFd(fd);
        this->deleteFromClientList(fd);
        //borrar tambien de los canales, y dentro de los canales, borrar de los usuarios y de los privilegios
        close(fd);
    }
    else
    {
        // Datos recibidos los guardamos en buffer para ser procesados mas adelante
        client->setBuffer(buffer);
        std::cout << "Data received: " << client->getBuffer() << std::endl;
        std::cout << "Client fd: " << fd << std::endl;  
        if (client->getBuffer().find_first_of("\r\n") != std::string::npos) //Si no devuelve npos, es que ha encontrado el string "\r\n"(fin de linea)
        {
            // Procesar los datos
            // A partir de aqui se procesan los datos, se parsean, se ejecutan los comandos, se envian mensajes, etc
            data = this->parseRecvData(client->getBuffer());
            for (size_t i = 0; i < data.size(); i++)
            {
                this->parseCommand(data[i], client->getClientFd());
            }
            if (this->getUserByFd(fd)) // Limpiar el buffer del cliente, si este sigue conectado
            {
                this->getUserByFd(fd)->getBuffer().clear();
            }
        }
        else
        {
            // se ha recibido un mensaje parcial, se espera a recibir el resto
            return;
        }        
    }   
}

std::vector<std::string> Server::parseRecvData(std::string buffer)
{
	std::vector<std::string> returnData;
	std::istringstream iss(buffer);
	std::string token;
    size_t pos = 0;
	while(std::getline(iss, token))
	{
		pos = token.find_first_of("\r\n");
		if(pos != std::string::npos)
			token = token.substr(0, pos);  // Eliminar el delimitador "\r\n"
		returnData.push_back(token);
	}
	return returnData;
}

void Server::deleteFromClientList(int fd)
{
    for (std::map<int, Client *>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
    {
        if (it->first == fd)
        {
            delete it->second;
            this->_users.erase(it);
            break;
        }
    }
}

void Server::deleteClientPollFd(int fd)
{
    std::cout << "Remove client from poll list-> fd: "
              << fd
              << ", ip: "
              << this->_users[fd]->getClientIp()
              << ", nick: "
              << this->_users[fd]->getNickname() << std::endl;
    for(size_t i = 0; i < this->_pollfds.size(); i++)
    {
        if (this->_pollfds[i].fd == fd)
        {
            this->_pollfds.erase(this->_pollfds.begin() + i);
            break;
        }
    }
}

/******************************************************************************
* ------------------------------- COMMAND ----------------------------------- *
******************************************************************************/

std::vector<std::string> Server::splitCmd(std::string& command) {
    std::vector<std::string> result;
    std::istringstream iss(command);
    std::string token;
    while (iss >> token)
    {
        result.push_back(token);
    }
    
    return result;
}

CommandType Server::getCommandType(const std::string& command) 
{
    if (command == "PASS" || command == "/pass")
        return CMD_PASS;
    if (command == "NICK" || command == "/nick")
        return CMD_NICK;
    if (command == "USER" || command == "/user")
        return CMD_USER;
    if (command == "QUIT" || command == "/quit")
        return CMD_QUIT;
    if (command == "KICK" || command == "/kick")
        return CMD_KICK;
    if (command == "JOIN" || command == "/join")
        return CMD_JOIN;
    if (command == "TOPIC" || command == "/topic")
        return CMD_TOPIC;
    if (command == "MODE" || command == "/mode")
        return CMD_MODE;
    if (command == "PRIVMSG" || command == "/privmsg")
        return CMD_PRIVMSG;
    if (command == "INVITE" || command == "/invite")
        return CMD_INVITE;
    return CMD_UNKNOWN;
}

void Server::printCmd(std::vector<std::string> &splited_cmd)
{
    for (size_t i = 0; i < splited_cmd.size(); i++)
    {
        std::cout << "splited_cmd[" << i << "]: " << splited_cmd[i] << std::endl;
    }
}

void Server::parseCommand(std::string &command, int fd)
{
    if (command.empty())
        return;
    std::vector<std::string> splited_cmd = splitCmd(command);
    size_t found = command.find_first_not_of(" \t\v");
    if (found != std::string::npos)
        command = command.substr(found);
    CommandType cmdType = CMD_UNKNOWN;
    if (splited_cmd.size() > 0) 
    {
        cmdType = getCommandType(splited_cmd[0]);
    }
    ACommand *commandHandler = NULL;
    switch (cmdType) 
    {
        case CMD_NICK:
            std::cout << "CMD_NICK" << std::endl;
            printCmd(splited_cmd);
            commandHandler = new Nick(*this);
            commandHandler->run(splited_cmd, fd);
            delete commandHandler;
            break;
        case CMD_USER:
            std::cout << "CMD_USER" << std::endl;
            printCmd(splited_cmd);
            commandHandler = new User(*this);
            commandHandler->run(splited_cmd, fd);
            delete commandHandler;
            break;
        case CMD_PASS:
            std::cout << "CMD_PASS" << std::endl;
            printCmd(splited_cmd);
            commandHandler = new Pass(*this);
            commandHandler->run(splited_cmd, fd);
            delete commandHandler;
            break;
        case CMD_QUIT:              
            std::cout << "CMD_QUIT" << std::endl;
            break;
        case CMD_KICK:
            std::cout << "CMD_KICK" << std::endl;
            break;
        case CMD_JOIN:
            std::cout << "CMD_JOIN" << std::endl;
            break;
        case CMD_TOPIC:
            std::cout << "CMD_TOPIC" << std::endl;
            break;
        case CMD_MODE:
            std::cout << "CMD_MODE" << std::endl;
            break;
        case CMD_PRIVMSG:
            std::cout << "CMD_PRIVMSG" << std::endl;
            break;
        case CMD_INVITE:
            std::cout << "CMD_INVITE" << std::endl;
            break;
        default:
                send(fd, "421 Unknown command\r\n", 21, 0);
            break;
    }    
}
