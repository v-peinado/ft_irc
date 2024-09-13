/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 12:21:04 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/13 16:45:11 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "Client.hpp"
#include "Channel.hpp"
#include "ACommand.hpp"
#include <exception>

class Server
{
    private:
        // Forma canonica no implementada
        Server(const Server &src);
        Server &operator=(const Server &src);

        // Server 
        int port;
        std::string serverName;
        std::string password;
        std::string welcomeMessage;
        std::string timeServer;
        bool active;
        
        // Server socket
        int serverFd;
        struct sockaddr_in serverAddr; // Server address, arpa/inet.h

        // Comandos
        
        // poll

        // Maps
            std::map<int , Client *> users; // Server users
            std::map<string::string , Channel *> channels; // Server channels
            std::map<string::string , ACommand *> commands; // Server commands
        // Server config(max users, max channels, etc)
           
    public:
        // Forma canonica implementada
        Server();
        ~Server();

        // Getters, el poner referencias constantes o no, es para que no se pueda modificar el valor, dependera del desarrollo posterior
            int getPort() const;
            std::string const &getServerName() const;
            std::string const &getPassword() const;
            std::string const &getWelcomeMessage() const;
            std::string const &getTimeServer() const;
            bool const &getActive() const;
            int const &getServerFd() const;
            struct sockaddr_in const &getServerAddr() const;
            AClient *getUserByFd(int fd);
            Channel *getChannelByName(std::string const &name);
            ACommand *getCommandByName(std::string const &name);
            std::map<int , AClient *> const &getUsers() const;
            std::map<string::string , Channel *> const &getChannels() const;
            std::map<string::string , ACommand *> const &getCommands() const;
        // Setters
            void setPort(int port);
            void setServerName(std::string const &serverName);
            void setPassword(std::string const &password);
            void setWelcomeMessage(std::string const &welcomeMessage);
            void setTimeServer(std::string const &timeServer);
            void setActive(bool active);
            void setServerFd(int serverFd);
            void setServerAddr(struct sockaddr_in serverAddr);

        // Server config, new channels, users, commands
            Channel * newChannel(std::string const &name);

        // Server config, insert channels, users, commands
            void insertUser(int fd, AClient *user);
            void insertChannel(std::string const &name, Channel *channel);
            void insertCommand(std::string const &name, ACommand *command);
            
        // Server config, delete channels, users, commands
            void deleteUser(int fd);
            void deleteChannel(std::string const &name);

        
        // Server start, stop, connection
            void startServer();
            void stopServer();
            void connection(); // Server connection
        // Server send, receive
        
            void sendToClient(int fd, std::string const &message);
            void receiveFromClient(int fd, std::string const &message);
            
        // Server poll                       
};
    
#endif

