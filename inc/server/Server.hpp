/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:21:04 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/16 16:10:52 by vpeinado         ###   ########.fr       */
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
#include <signal.h>
#include "Client.hpp"
#include "Channel.hpp"
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
    CMD_UNKNOWN,
    CMD_INFO,
    CMD_PING,
    CMD_CAP
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
        std::string _serverHost;
        static bool _active;
        
        /*** SOCKETS and POLL ***/
        
        int _serverFd;
        struct sockaddr_in _serverAddr;
        std::vector<struct pollfd> _pollfds;
        
        /*** USERS ***/
        
        std::map<int , Client *> _users;

        /*** CHANNELS ***/

        std::map<std::string, Channel *> _channels;     //nueva
         
    public:
        
        /*** Orthodox Canonical Form ***/
        
        Server(char *port, char *password);
        ~Server();

        /*** GETTERS ***/
        
        std::string const &getServerName() const;
        std::map<int , Client *> const &getUsers() const;
        Client *getUserByFd(int fd);
        Client *getUserByNick(std::string nick);
        std::string const &getPassword() const;
        std::string &getHost();
             
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
        void deleteFromAllChannels(int fd);

        /*** CHANNELS ***/
        
        Channel *getChannelByName(std::string channelName);     //nueva
        std::map<std::string, Channel *> &getChannels();       //nueva
        
        /*** COMMANDS ***/
        
        CommandType getCommandType(const std::string& command);
        void parseCommand(std::string &command, int fd);
        std::vector<std::string> splitCmd(std::string &command);
        void printCmd(std::vector<std::string> &splited_cmd);
        void printInfo();

        /*** SIGNALS ***/

        static void signalHandler(int signal);

        /*** SEND ***/

        void sendError(int code, std::string clientname, std::string channelname, int fd, std::string msg);
        void sendResponse(std::string response, int fd);
                                  
};
    
#endif

