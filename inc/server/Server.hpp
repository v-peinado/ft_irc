/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:21:04 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/17 00:20:35 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Client.hpp"
#include "Channel.hpp"
//#include "ACommand.hpp"
#include <exception>
#include <vector>
#include <poll.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>


#define MAX_CLIENTS 10

class Server
{
    private:
        // Forma canonica no implementada
        Server(const Server &src);
        Server &operator=(const Server &src);

        // Server 
        int _port;
        std::string _serverName;
        std::string _password;
        std::string _welcomeMessage;
        bool _active;
        
        // Server socket
        int _serverFd;
        struct sockaddr_in _serverAddr; // Server address, arpa/inet.h

        // Comandos
        
        // poll, lista de fd que poll va a monitorear
        std::vector<struct pollfd> _pollfds;

        // Maps
            std::map<int , Client *> _users; // Server users
           // std::map<std::string , Channel *> _channels; // Server channels
           // std::map<std::string , ACommand *> _commands; // Server commands
        // Server config(max users, max channels, etc)
           
    public:
        // Forma canonica implementada
        Server();
        ~Server();

        // Getters, el poner referencias constantes o no, es para que no se pueda modificar el valor, dependera del desarrollo posterior
            std::string const &getServerName() const;
            // Channel *getChannelByName(std::string const &name);
            // ACommand *getCommandByName(std::string const &name);
            std::map<int , Client *> const &getUsers() const;
            //  std::map<std::string , Channel *> const &getChannels() const;
            //  std::map<std::string , ACommand *> const &getCommands() const;
        // Setters

            void setPollfds(pollfd pollfd);
            void setWelcomeMessage();
            
        // Server config, new channels, users, commands
            //  Channel * newChannel(std::string const &name);

        // Server config, insert channels, users, commands
            //void insertUser(int fd, Client *user);
            //  void insertChannel(std::string const &name, Channel *channel);
            //  void insertCommand(std::string const &name, ACommand *command);
            
        // Server config, delete channels, users, commands
            void deleteUser(int fd);
            void deleteChannel(std::string const &name);

        // Server start, stop, connection
            void startServer(char *port, char *password);
            void printServerInfo();
            void runServer();
            void stopServer();
            void newClientConnection(); // Server connection
            void reciveNewData(int fd); // Server receive data           
        // Server poll                       
};
    
#endif

