/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 15:53:24 by vpeinado          #+#    #+#             */
/*   Updated: 2025/05/08 22:17:12 by vpeinado         ###   ########.fr       */
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
#include "Invite.hpp"
#include "Mode.hpp"
#include "Topic.hpp"

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

std::string &Server::getHost()
{
    return this->_serverHost;
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
    if (pollfd.fd < 3)                                                    // 0, 1, 2 : reserved file descriptors
    {
        throw std::runtime_error("Error: invalid fd");
        return;
    }
    this->_pollfds.push_back(pollfd);                                     // Add the pollfd to the vector of pollfds
}

void Server::setWelcomeMessage()
{
    this->_welcomeMessage = 
        "*************************************************\n"
        "*          Welcome to the IRC Server!           *\n"
        "*   1. Use PASS <password> to log in            *\n"
        "*   2. Use NICK <nickname> to set your nickname *\n"  
        "*   3. Use USER <username> 0 * :<realname>      *\n"
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
    setSocket();                  // Create the socket

    configServerAddr();           // Configure the server address
    
    setSocketOptions();           // Option 1: Reuse the server address

    bindSockets();                // Bind the socket to the given port

    listenAndPoll();              // Listen and monitor socket events
}

void Server::setSocket()
{
    this->_serverFd = socket(AF_INET, SOCK_STREAM, 0);           // Create the socket, AF_INET = IPv4, SOCK_STREAM = TCP, 0 = Default protocol
    if (this->_serverFd < 0)
    {
        throw std::runtime_error("Error: socket failed");
        exit(1);
    }
}

void Server::configServerAddr()                                      // sockaddr_in, used to define an IPv4 socket address, including the port and IP address
{                                    
    std::memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));   // Clear the server address structure
    this->_serverAddr.sin_family = AF_INET;                          // Address family, AF_INET = IPv4
    this->_serverAddr.sin_addr.s_addr = INADDR_ANY;                  // Accept connections from any IP address, 0.0.0.0              
    this->_serverAddr.sin_port = htons(this->_port);                 // Server port, htons() converts the unsigned short integer from host to network byte order
    this->_serverHost = "127.0.0.1";                                 // Server IP address, localhost
}

void Server::setSocketOptions()
{
    int opt = 1;                                                                           // Option to reuse the server address
    if(setsockopt(this->_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)       // SOL_SOCKET = Socket level, SO_REUSEADDR = Reuse server address
    {
        throw std::runtime_error("Error: setsockopt");
        close(this->_serverFd);
        exit(1);
    }
    if(fcntl(this->_serverFd, F_SETFL, O_NONBLOCK) < 0)                                     // Set fd to be non-blocking, they won't wait for the operation to complete if no data is available
    {
        throw std::runtime_error("Error: fcntl");
        close(this->_serverFd);
        exit(1);
    }
}

void Server::bindSockets()
{
    if(bind(this->_serverFd, (struct sockaddr *)&this->_serverAddr, sizeof(this->_serverAddr)) < 0)     // Bind the socket to the given port
    {
        throw std::runtime_error("Error: bind");
        close(this->_serverFd);
        exit(1);
    }
}

void Server::listenAndPoll()
{
    if(listen(this->_serverFd, MAX_CLIENTS) < 0)        // Listen for incoming connections, that is, wait for clients to connect    
    {
        throw std::runtime_error("Error: listen");
        close(this->_serverFd);
        exit(1);
    }
    pollfd serverPollFd;                               // pollfd structure, monitor socket events
    serverPollFd.fd = this->_serverFd;                 // Server file descriptor
    serverPollFd.events = POLLIN;                      // Events to monitor, POLLIN = Data ready to read
    serverPollFd.revents = 0;                          // Events that occurred, initialized to 0
    this->setPollfds(serverPollFd);                    // Add the server pollfd to the vector of pollfds that contains the clients to monitor
}

void Server::runServer()
{
    while(Server::_active)                                                  // Main server loop, runs while the server is active
    {                                                                      
        if ((poll(this->_pollfds.data(), this->_pollfds.size(), -1) < 0) 
            && Server::_active)                                             // poll() monitors socket events, -1 = Wait indefinitely, called on each loop iteration
        {                                                                   // monitoring changes in file descriptors on each iteration
            throw std::runtime_error("Error: poll");
            close(this->_serverFd);
            exit(1);
        }

        for (size_t i = 0; i < this->_pollfds.size(); i++)                  // Iterate through all elements in the vector of pollfds
        {                                                                   // Wait 1 ms, to avoid consuming too many system resources
            if (this->_pollfds[i].revents & POLLIN)                         // If the result of the & operation is true(1), 0x01 = 0x01, POLLIN = POLLIN the condition is true
            {                                                               // POLLIN is a bitmask that indicates there is data to read and the & operation
                if (this->_pollfds[i].fd == this->_serverFd)                // Check if the socket with events is the server socket, indicating that a new client is trying to connect
                    this->newClientConnection();                            // If it's the server socket, call the function to accept new client connections
                else
                    this->reciveNewData(this->_pollfds[i].fd);              // If the active socket is not the server, it means data is being received from an already connected client
            }
        }
    }
    stopServer();
}

void Server::stopServer()
{      
    // close client sockets and free memory
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

void Server::newClientConnection()                                                                              
{
    Client *newClient = new Client();                                                                           // Create a new client
    memset(&newClient->getClientAddr(), 0, sizeof(newClient->getClientAddr()));                                 // Clear the client address structure
    socklen_t addrLen = sizeof(newClient->getClientAddr());                                                     // Length of client address

    int newClientFd = accept(this->_serverFd, (struct sockaddr *)&newClient->getClientAddr(), &addrLen);        // Accept client connection, returns the fd of the connecting client
    if (newClientFd < 0)                                                                                        // If the fd is less than 0, there has been an error
    {
        throw std::runtime_error("Error: accept");
        delete newClient; 
        return;
    }
                                                                                                                // Configure the new client, get its IP address, fd.. from the pollfds list
    newClient->getClientPollFd().fd = newClientFd;                                                              // Set the client's file descriptor in the client's pollfd structure                                           
    newClient->getClientPollFd().events = POLLIN;                                                               // POLLIN = Data ready to be read from the client socket without blocking, when monitoring with poll()
    newClient->getClientPollFd().revents = 0;                                                                   // Events that occurred, initialized to 0
    newClient->setClientFd(newClientFd);                                                                        // Client file descriptor
    newClient->setClientIp(inet_ntoa(newClient->getClientAddr().sin_addr));                                     // Client IP address, we use the inet_ntoa function to convert the IP address to a string
    newClient->setRegistered(false);                                                                            // The client is not registered

    this->setPollfds(newClient->getClientPollFd());                                                             // Add the client's pollfd to the vector of pollfds that contains the clients to monitor

    this->_users.insert(std::make_pair(newClientFd, newClient));                                                // Insert the new client in the client list               

    std::cout << "New client connected: " << newClient->getClientIp() << std::endl;                             // Print the IP address of the client that has connected, on the server

    send(newClientFd, this->_welcomeMessage.c_str(), this->_welcomeMessage.size(), 0);                          // Send the welcome message to the client that has connected  
}

void Server::reciveNewData(int fd)
{
    std::vector<std::string> data;                                                  // We will use the vector to store possible commands contained in a single message
    char buffer[1024];                                                              // Buffer to receive data, needed because recv cannot receive a string directly
    memset(buffer, 0, 1024);                                                        // Clean buffer
    Client *client = this->_users[fd];                                              // Represents the client sending the data, which is in the user list
    int bytes = recv(fd, buffer, 1024, 0);                                          // Receive data from the client, returns the number of bytes received  
    if (bytes < 0)                                                                  // If recv returns a negative value, there has been an error
    {    
         throw std::runtime_error("Error: recv");
    }
    else if (bytes == 0)                                                            // If recv returns 0, the client has disconnected
    {
        std::cout << "Client disconnected, fd: " << fd << std::endl;
        this->deleteClientPollFd(fd);
        this->deleteFromAllChannels(fd);                                            // Delete the client from lists, structures, etc.  
        this->deleteFromClientList(fd);       
        close(fd);
    }
    else                                                                            // If recv returns a positive value, data has been received
    {
        client->setBuffer(buffer);                                                  // Save the data in the client buffer, the data will accumulate, since TCP does not guarantee that they arrive complete in a single packet
        std::cout << "Data received: " << client->getBuffer() << std::endl;
        std::cout << "Client fd: " << fd << std::endl;  
        if (client->getBuffer().find_first_of("\r\n") != std::string::npos)         // If it doesn't return npos, it means it has found the string "\r\n" (end of line), which is what delimits messages in IRC
        {                                                                           // and we need to split the commands
            data = this->parseRecvData(client->getBuffer());                        // We will split using "\r\n" as a delimiter, which is what separates commands in IRC
            for (size_t i = 0; i < data.size(); i++)                                // Iterate through the commands stored in the vector, send them to be parsed one by one
            {
                this->parseCommand(data[i], client->getClientFd());                 // Parse the commands
            }
            if (this->getUserByFd(fd))                                              // Clear the client buffer, if the client is still connected
            {    
                this->getUserByFd(fd)->getBuffer().clear();                         // we only clear it because we will continue interacting with the client                                                          
            }
        }
        else
        {
            // a partial message has been received, waiting to receive the rest
            return;
        }        
    }   
}

std::vector<std::string> Server::parseRecvData(std::string buffer)  // Parse the received data 
{
    std::vector<std::string> returnData;                            // Vector to store commands and arguments
    std::istringstream iss(buffer);                                 // Convert the buffer to a stream
    std::string token;                                              // We will tokenize the buffer
    size_t pos = 0;                                                 // Position of the token
    while(std::getline(iss, token))                                 // Iterate through the stream, tokenizing the data
    {
        pos = token.find_first_of("\r\n");                          // Search for the string "\r\n" in the token      
        if(pos != std::string::npos)                                // If it doesn't return npos, it means it has found the string "\r\n" (end of line)
            token = token.substr(0, pos);                           // Copy the token until it finds the string "\r\n"
        returnData.push_back(token);                                // Add the token, which contains a command and its arguments, to the vector
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

void Server::deleteFromAllChannels(int fd)
{
    std::map<std::string, Channel *> canales = this->getChannels();
    std::map<std::string, Channel *>::iterator it = canales.begin();
    while (it != canales.end())
    {
        if (it->second->isClientInChannel(fd))
            it->second->removeClient(fd);
        if (it->second->isClientAdmin(fd))
            it->second->removeAdmin(fd);
        if (it->second->isClientInvited(fd))
            it->second->removeInvitedClient(fd);
        it++;
    }
    
}

bool Server::ifClientExist(std::string nick)
{
    for (std::map<int, Client *>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
    {
        if (it->second->getNickname() == nick)
            return true;
    }
    return false;
}

/******************************************************************************
* ------------------------------- COMMAND ----------------------------------- *
******************************************************************************/

std::vector<std::string> Server::splitCmd(std::string& command) // We will split the command by spaces
{
    std::vector<std::string> result;                            // Vector to store commands and arguments
    std::istringstream iss(command);                            // Convert the string to a stream
    std::string token;                                          // Token for each "word" of the command             
    while (iss >> token)                                        // Iterate through the stream, tokenizing the data, by default the delimiter is the space
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
    std::endl(std::cout);
}
void Server::printInfo()
{   
    //print channels list
    std::cout << "Channels: " << std::endl;
    for (std::map<std::string, Channel *>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
    {
        std::cout << "Channel: " << it->first << std::endl;
        std::cout << "Key: " << it->second->GetKey() << std::endl;
        //print user list
        std::cout << "Users: " << std::endl;
        for (size_t i = 0; i < it->second->GetClients().size(); i++)
            std::cout << "[" << i << "]User: " << it->second->GetClients()[i]->getNickname() << std::endl;
        std::cout << "Admins: " << std::endl;
        for (size_t i = 0; i < it->second->GetAdmins().size(); i++)
            std::cout << "[" << i << "]Admin: " << it->second->GetAdmins()[i]->getNickname() << std::endl;
        std::cout << "Invited: " << std::endl;
        for (size_t i = 0; i < it->second->GetInvitedClients().size(); i++)
            std::cout << "[" << i << "]Invited: " << it->second->GetInvitedClients()[i] << std::endl;
    }  
}
void Server::parseCommand(std::string &command, int fd)
{
    if (command.empty())                                            // Check if the command is empty  
        return;
    // if (this->getUserByFd(fd)->getUsername().empty())            // Check if the user is connected, maybe within each command                        
    // {
    //     std::string channelName = "";
    //     this->sendError(451, 
    //             this->getUserByFd(fd)->getNickname(), 
    //             channelName, fd, " :You have not registered\r\n");        
    //     return;
    // }
    std::vector<std::string> splited_cmd = splitCmd(command);       // Split the command by spaces
    CommandType cmdType = CMD_UNKNOWN;                              // Command type, a variable of CommandType(enum) type, initialized to CMD_UNKNOWN
    if (splited_cmd.size() > 0)                                     // If the split command has more than 0 elements
    {
        cmdType = getCommandType(splited_cmd[0]);                   // Get the command type, the first element of the split command is compared with the known commands from enum CommandType
    }
    ACommand *commandHandler = NULL;                                // Pointer to the abstract class ACommand, initialized to NULL, we will use it to create command objects
    switch (cmdType)                                                
    {
        case CMD_CAP:
            break;
        case CMD_INFO:
            printInfo();
            break;
        case CMD_NICK:
            std::cout << "CMD_NICK" << std::endl;
            printCmd(splited_cmd);                                  // Print the split command
            commandHandler = new Nick(*this);                       // Create an object of the corresponding class, passing the server to it
            commandHandler->run(splited_cmd, fd);                   // Call the run method, which is the pure virtual method of the abstract class ACommand
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
            printCmd(splited_cmd); 
            commandHandler = new Topic(*this);
            commandHandler->run(splited_cmd, fd);
            delete commandHandler;
            break;
        case CMD_MODE:
            std::cout << "CMD_MODE" << std::endl;
            printCmd(splited_cmd); 
            commandHandler = new Mode(*this);
            commandHandler->run(splited_cmd, fd);
            delete commandHandler;
            break;
        case CMD_PRIVMSG:
            std::cout << "CMD_PRIVMSG" << std::endl;
            printCmd(splited_cmd); 
            commandHandler = new Privmsg(*this);
            commandHandler->run(splited_cmd, fd);
            break;
        case CMD_INVITE:
            std::cout << "CMD_INVITE" << std::endl;
            printCmd(splited_cmd); 
            commandHandler = new Invite(*this);
            commandHandler->run(splited_cmd, fd);
            break;
        default:
            std::string response = ": 421 " + this->getUserByFd(fd)->getNickname() + splited_cmd[0] + " :Unknown command\r\n";
            send(fd, response.c_str(), response.size(), 0);
            break;
    }    
}

/******************************************************************************
* ------------------------------- SIGNALS ----------------------------------- *
******************************************************************************/

bool Server::_active = true;                                           // Static variable, because it is used in a static method
void Server::signalHandler(int signum)                                 // Signal handler, called when a signal is received, also static
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

bool Server::channelExist(std::string channelName)
{
    std::map<std::string, Channel *>::iterator it = this->_channels.find(channelName);
    if (it != this->_channels.end())
        return true;
    else
        return false;
}

/******************************************************************************
* ------------------------------- SEND -------------------------------------- *
******************************************************************************/


void Server::sendError(int code, std::string clientname, std::string channelname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":" << this->_serverHost << " " << code << " " << clientname << " " << channelname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "Error send() fail" << std::endl;
}

void Server::sendError(int code, std::string clientname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":" << this->_serverHost << " " << code << " " << clientname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "send() faild" << std::endl;
}

void Server::sendResponse(std::string response, int fd)
{
	if(send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() fail" << std::endl;
}