/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:21:04 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/21 13:40:02 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>
#include <vector>
#include <poll.h>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include "Client.hpp"
#include <cstdio>

#define MAX_CLIENTS 10

enum CommandType {
    CMD_PASS,
    CMD_NICK,
    CMD_USER,
    CMD_QUIT,
    CMD_KICK,
    CMD_JOIN,
    CMD_TOPIC,
    CMD_MODE,
    CMD_PRIVMSG,
    CMD_INVITE,
    CMD_UNKNOWN
};

class Server
{
    private:
    
        /*** Orthodox Canonical Form ***/
        
        Server();
        Server(const Server &src);
        Server &operator=(const Server &src);

        /*** SERVER ***/
        
        int _port;
        std::string _serverName;
        std::string _password;
        std::string _welcomeMessage;
        bool _active;
        
        /*** SOCKETS and POLL ***/
        
        int _serverFd;
        struct sockaddr_in _serverAddr;
        std::vector<struct pollfd> _pollfds;
        
        /*** USERS ***/
        
        std::map<int , Client *> _users;
         
    public:
        
        /*** Orthodox Canonical Form ***/
        
        Server(char *port, char *password);
        ~Server();

        /*** GETTERS ***/
        
        std::string const &getServerName() const;
        std::map<int , Client *> const &getUsers() const;
        Client *getUserByFd(int fd);
        std::string const &getPassword() const;
        
            
        /*** SETTERS ***/

        void setPollfds(pollfd pollfd);
        void setWelcomeMessage();
                       
        /*** SERVER ***/
        
        void startServer();
        void setSocket();
        void configServerAddr();
        void setSocketOptions();
        void bindSockets();
        void listenAndPoll();
        void printServerInfo();
        void runServer();
        void stopServer();


        /*** CLIENTS ***/
        
        void newClientConnection();
        void reciveNewData(int fd);
        std::vector<std::string> parseRecvData(std::string buffer);
        void deleteFromClientList(int fd);
        void deleteClientPollFd(int fd); 
        
        /*** COMMANDS ***/
        
        CommandType getCommandType(const std::string& command);
        void parseCommand(std::string &command, int fd);
        std::vector<std::string> splitCmd(std::string &command);
        void printCmd(std::vector<std::string> &splited_cmd);                          
};
    
#endif

