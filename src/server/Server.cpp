/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:53:24 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/03 20:11:38 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Pass.hpp"
#include "User.hpp"
#include "Nick.hpp"
#include "Join.hpp"
#include "Privmsg.hpp"
#include "Kick.hpp"
#include "Ping.hpp"

/******************************************************************************
* ------------------------------- CONSTRUCTORS ------------------------------ *
******************************************************************************/

Server::Server(char *port, char *password)
{
    this->_serverName = "IRCserv: ffons-ti & vpeinado";
    this->_port = atoi(port);
    this->_password = password;
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

Client *Server::getUserByNick(std::string nick)
{
    for (std::map<int, Client *>::iterator it = this->_users.begin(); it != this->_users.end(); ++it)
    {
        if (it->second->getNickname() == nick)
            return it->second;
    }
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
    if(listen(this->_serverFd, MAX_CLIENTS) < 0)        // Escuchar las conexiones entrantes, es decir, esperar a que los clientes se conecten    
    {
        throw std::runtime_error("Error: listen");
        close(this->_serverFd);
        exit(1);
    }
    pollfd serverPollFd;                               // Estructura pollfd, monitorear los eventos de los sockets
    serverPollFd.fd = this->_serverFd;                 // File descriptor del servidor
    serverPollFd.events = POLLIN;                      // Eventos a monitorear, POLLIN = Datos listos para leer
    serverPollFd.revents = 0;                          // Eventos que ocurrieron, inicializado a 0
    this->setPollfds(serverPollFd);                    // Añadir el pollfd del servidor al vector de pollfds que contiene los clientes a monitorear
}

void Server::runServer()
{
    while(Server::_active)                                                  // Bucle principal del servidor, se ejecuta mientras el servidor este activo
    {                                                                      
        if ((poll(this->_pollfds.data(), this->_pollfds.size(), -1) < 0) 
            && Server::_active)                                             // poll() monitorea los eventos de los sockets, -1 = Esperar indefinidamente, se llama en cada iteracion del bucle
        {                                                                   // monitoreando los cambios en los descriptores de archivo cada iteracion
            throw std::runtime_error("Error: poll");
            close(this->_serverFd);
            exit(1);
        }

        for (size_t i = 0; i < this->_pollfds.size(); i++)                  // Itera a través de todos los elementos del vector de pollfds
        {                                                                   // Esperar 1 ms, para no consumir muchos recursos del sistema
            if (this->_pollfds[i].revents & POLLIN)                         // Si el fd tiene la flag POLLIN activada, significa que hay datos listos para ser leidos
            {
                if (this->_pollfds[i].fd == this->_serverFd)                // Comprueba si el socket con eventos es el socket del servidor, lo que indica que un nuevo cliente está intentando conectarse
                    this->newClientConnection();                            // Si es el socket del servidor, llama a la función para aceptar nuevas conexiones de clientes
                else
                    this->reciveNewData(this->_pollfds[i].fd);              // Si el socket activo no es el  servidor, significa que se reciben datos de un cliente ya conectado
            }
        }
    }
    stopServer();
}

void Server::stopServer()
{      

    // cerrar los socketa de los clientes y liberar la memoria
    for (std::map<int, Client *>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
    {
        send(it->first, "Server is shutting down\r\n", 26, 0);
        delete it->second;
    }    
    for (size_t i = 0; i < _pollfds.size(); i++) {
        if (_pollfds[i].fd != _serverFd) {   
            close(_pollfds[i].fd);      
        }
    }
    close(_serverFd);
    // Log or print a message indicating the server has been shut down
    std::cout << "Server stopped and all connections closed." << std::endl;   
}

/******************************************************************************
* ------------------------------- CLIENTS ----------------------------------- *
******************************************************************************/

void Server::newClientConnection()                                                                              // Aceptar nuevas conexiones de clientes
{
    Client *newClient = new Client();                                                                           // Crear un nuevo cliente
    memset(&newClient->getClientAddr(), 0, sizeof(newClient->getClientAddr()));                                 // Limpiar la estructura de la direccion del cliente
    socklen_t addrLen = sizeof(newClient->getClientAddr());                                                     // Longitud de la direccion del cliente

    int newClientFd = accept(this->_serverFd, (struct sockaddr *)&newClient->getClientAddr(), &addrLen);        // Aceptar la conexion del cliente, devuelve el fd del cliente que se conecta
    if (newClientFd < 0)                                                                                        // Si el fd es menor que 0, ha habido un error
    {
        throw std::runtime_error("Error: accept");
        delete newClient; // Asegurarse de liberar la memoria en caso de error
        return;
    }
                                                                                                                // Configurar el nuevo cliente, obtener su direccion IP, fd.. de la lista pollfds
    newClient->getClientPollFd().fd = newClientFd;                                                              // Establecer el file descriptor del cliente dentro de la estructura pollfd del cliente                                           
    newClient->getClientPollFd().events = POLLIN;                                                               // POLLIN = Datos listos para ser leídos en el socket del cliente sin bloquear, cuando monitoreamos con poll()
    newClient->getClientPollFd().revents = 0;                                                                   // Eventos que ocurrieron, inicializado a 0
    newClient->setClientFd(newClientFd);                                                                        // File descriptor del cliente
    newClient->setClientIp(inet_ntoa(newClient->getClientAddr().sin_addr));                                     // Direccion IP del cliente, usamos la funcion inet_ntoa para convertir la direccion IP a una cadena
    newClient->setRegistered(false);                                                                            // El cliente no esta registrado

    this->setPollfds(newClient->getClientPollFd());                                                             // Añadir el pollfd del cliente al vector de pollfds que contiene los clientes a monitorear                    

    this->_users.insert(std::make_pair(newClientFd, newClient));                                                // Insertar el nuevo cliente en la lista de clientes                

    std::cout << "New client connected: " << newClient->getClientIp() << std::endl;                             // Imprimir la direccion IP del cliente que se ha conectado, en el server

    send(newClientFd, this->_welcomeMessage.c_str(), this->_welcomeMessage.size(), 0);                          // Enviar el mensaje de bienvenida al cliente que se ha conectado   
}

void Server::reciveNewData(int fd)
{
    std::vector<std::string> data;                                                  // Usaremos el vector para guardar los posibles comandos almacenados en un solo mensaje
    char buffer[1024];                                                              // Buffer para recibir los datos, es neceseario porque recv no puede recibir un string directamente
    memset(buffer, 0, 1024);                                                        // Limpiar el buffer
    Client *client = this->_users[fd];                                              // Representa al cliente que envia los datos, que esta en la lista de usuarios
    int bytes = recv(fd, buffer, 1024, 0);                                          // Recibir los datos del cliente, devuelve el numero de bytes recibidos  
    if (bytes < 0)                                                                  // Si recv devuelve un valor negativo, ha habido un error
    {    
         throw std::runtime_error("Error: recv");
    }
    else if (bytes == 0)                                                            // Si recv devuelve 0, el cliente se ha desconectado
    {
        std::cout << "Client disconnected, fd: " << fd << std::endl;
        this->deleteClientPollFd(fd);                                               // Borrar el cliente de la listas, estructuras, etc  
        this->deleteFromClientList(fd);
        //borrar tambien de los canales, y dentro de los canales, borrar de los usuarios y de los privilegios
        close(fd);
    }
    else                                                                            // Si recv devuelve un valor positivo, se han recibido datos
    {
        client->setBuffer(buffer);                                                  // Guardar los datos en el buffer del cliente se iran acumulando los datos, ya que tcp no garantiza que lleguen completos en un solo paquete
        std::cout << "Data received: " << client->getBuffer() << std::endl;
        std::cout << "Client fd: " << fd << std::endl;  
        if (client->getBuffer().find_first_of("\r\n") != std::string::npos)         //Si no devuelve npos, es que ha encontrado el string "\r\n"(fin de linea), que es lo que delimita los mensajes en IRC
        {                                                                           // y debemos splitear los comandos
            data = this->parseRecvData(client->getBuffer());                        // Splitearemos usando como delimitador "\r\n", que es lo que separa los comandos en IRC
            for (size_t i = 0; i < data.size(); i++)                                // Iterar a través de los comandos al macenados en el vector, mandarlos a parsear uno a uno
            {
                this->parseCommand(data[i], client->getClientFd());                 // Parsear los comando
            }
            if (this->getUserByFd(fd))                                              // Limpiar el buffer del cliente, si este sigue conectado
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

std::vector<std::string> Server::parseRecvData(std::string buffer)  // Parsear los datos recibidos 
{
	std::vector<std::string> returnData;                            // Vector para almacenar los comandos y argumentos
	std::istringstream iss(buffer);                                 // Convertir el buffer en un stream
	std::string token;                                              // Tokenizaremos el buffer
    size_t pos = 0;                                                 // Posicion del token
	while(std::getline(iss, token))                                 // Iterar a través del stream, tokenizando los datos
	{
		pos = token.find_first_of("\r\n");                          // Buscar el string "\r\n" en el token      
		if(pos != std::string::npos)                                // Si no devuelve npos, es que ha encontrado el string "\r\n"(fin de linea)
			token = token.substr(0, pos);                           // Copiare el token hasta que encuentre el string "\r\n"
		returnData.push_back(token);                                // Añadir el token, que contiene un comando y sus argumentos, al vector
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

std::vector<std::string> Server::splitCmd(std::string& command) // Splitearemso el comando por los espacios
{
    std::vector<std::string> result;                            // Vector para almacenar los comandos y argumentos
    std::istringstream iss(command);                            // Convertir el string en un stream
    std::string token;                                          // Token de cada "palabra" del comando             
    while (iss >> token)                                        // Iterar a través del stream, tokenizando los datos, por defecto el delimitador es el espacio
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
    if (command == "INFO" || command == "/info")
        return CMD_INFO;
    if (command == "PING" || command == "/ping")
        return CMD_PING;
    if (command == "CAP" || command == "/cap")
        return CMD_CAP;
    return CMD_UNKNOWN;
}

void Server::printCmd(std::vector<std::string> &splited_cmd)
{
    for (size_t i = 0; i < splited_cmd.size(); i++)
    {
        std::cout << "splited_cmd[" << i << "]: " << splited_cmd[i] << std::endl;
    }
}
void Server::printInfo()
{
    //printar lista de canales
    std::cout << "Channels: " << std::endl;
    for (std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
    {
        std::cout << "Channel: " << it->first << std::endl;
        //printar lista de usuarios
        std::cout << "Users: " << std::endl;
        for (size_t i = 0; i < it->second->GetClients().size(); i++)
            std::cout << "[" << i << "]User: " << it->second->GetClients()[i]->getNickname() << std::endl;
        std::cout << "Admins: " << std::endl;
        for (size_t i = 0; i < it->second->GetAdmins().size(); i++)
            std::cout << "[" << i << "]Admin: " << it->second->GetAdmins()[i]->getNickname() << std::endl;
    }
    //printar lista de usuarios y sus canales    
}
void Server::parseCommand(std::string &command, int fd)
{
    if (command.empty())                                            // Comprobamos si el comando esta vacio    
        return;
    std::vector<std::string> splited_cmd = splitCmd(command);       // Splitear el comando por los espacios
    CommandType cmdType = CMD_UNKNOWN;                              // Tipo de comando, una variable de tipo CommandType(enum), inicializado a CMD_UNKNOWN
    if (splited_cmd.size() > 0)                                     // Si el comando spliteado tiene mas de 0 elementos
    {
        cmdType = getCommandType(splited_cmd[0]);                   // Obtener el tipo de comando, el primer elemento del comando spliteado se comapara con los comandos conocidos de enum CommandType
    }
    ACommand *commandHandler = NULL;                                // Puntero a la clase abstracta ACommand, inicializado a NULL, lo usaremos para crear los objetos de los comandos
    switch (cmdType)                                                
    {
        case CMD_CAP:
            break;
        case CMD_INFO:
            printInfo();
            break;
        case CMD_NICK:
            std::cout << "CMD_NICK" << std::endl;
            printCmd(splited_cmd);                                  // Imprimir el comando spliteado
            commandHandler = new Nick(*this);                       // Crear un objeto de la clase correspodiente, pasandole el servidor
            commandHandler->run(splited_cmd, fd);                   // Llamar al metodo run, que es el metodo virtual pura de la clase abstracta ACommand
            delete commandHandler;
            break;
        case CMD_USER:
            std::cout << "CMD_USER" << std::endl;
            printCmd(splited_cmd);
            commandHandler = new User(*this);
            commandHandler->run(splited_cmd, fd);
            delete commandHandler;
            break;
       case CMD_PING:
            std::cout << "CMD_PING" << std::endl;
            printCmd(splited_cmd);
            commandHandler = new Ping(*this);
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
            printCmd(splited_cmd);
            commandHandler = new Kick(*this);
            commandHandler->run(splited_cmd, fd);
            delete commandHandler;
            break;
        case CMD_JOIN:
            std::cout << "CMD_JOIN" << std::endl;
            printCmd(splited_cmd); 
            commandHandler = new Join(*this);
            commandHandler->run(splited_cmd, fd);
            delete commandHandler;
            break;
        case CMD_TOPIC:
            std::cout << "CMD_TOPIC" << std::endl;
            break;
        case CMD_MODE:
            std::cout << "CMD_MODE" << std::endl;
            break;
        case CMD_PRIVMSG:
            std::cout << "CMD_PRIVMSG" << std::endl;
            printCmd(splited_cmd); 
            commandHandler = new Privmsg(*this);
            commandHandler->run(splited_cmd, fd);
            break;
        case CMD_INVITE:
            std::cout << "CMD_INVITE" << std::endl;
            break;
        default:
            send(fd, "421 Unknown command2\r\n", strlen("421 Unknown command2\r\n"), 0);  //falla su impresion
            break;
    }    
}

/******************************************************************************
* ------------------------------- SIGNALS ----------------------------------- *
******************************************************************************/

bool Server::_active = true;                                           // Variable estatica, porque se usa en un metodo estatico
void Server::signalHandler(int signum)                                 // Manejador de señales, se llama cuando se recibe una señal, tambien estatico
{
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    Server::_active = false;
}

/******************************************************************************
* ------------------------------- CHANNELS ---------------------------------- *
******************************************************************************/

std::map<std::string, Channel *> &Server::getChannels()
{
    return this->_channels;
}

Channel *Server::getChannelByName(std::string channelName)
{
    std::map<std::string, Channel *>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end())
        return it->second;
    else
        return NULL;
}