/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:27:27 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/13 18:01:52 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <exception>

class Client
{
    private:
        //Forma canonica no implementada
        Client();
        Client(const Client &src);
        Client &operator=(const Client &src);

        // Client
        std::string nickname;
        std::string username;
        std::string realame;
        bool registered; // si su estado cambia por un comando de registro, o por un baneao, etc
        bool active; // Si esta conectado o no, si pasa mucho tiempo sin actividad se desconecta o se le manda un ping, o se pone en su canal de bienvenida,etc
        int level; // Nivel de permisos, si es admin, si es operador, si es usuario normal, etc

        // Client socket
        int clientFd;
        struct sockaddr_in clientAddr; // Server address, arpa/inet.h
        
        // Maps
        std::map<std::string, Channel *> channels; // Client channels

    public:
        // Forma canonica implementada
        Client(struct sockaddr_in client, int fd);
        ~Client();

        // Getters, el poner referencias constantes o no, es para que no se pueda modificar el valor, dependera del desarrollo posterior
        std::string const &getNickname() const;
        std::string const &getUsername() const;
        std::string const &getRealname() const;
        bool const &getRegistered() const;
        bool const &getActive() const;
        int const &getLevel() const;
        std::map<std::string, Channel *> const &getChannels() const;
        Channel *getChannelByName(std::string const &name) const;
        int getClientFd() const;
        struct sockaddr_in getClientAddr() const;

        // Setters
        void setNickname(std::string const &nickname);
        void setUsername(std::string const &username);
        void setRealname(std::string const &realname);
        void setRegistered(bool registered);
        void setActive(bool active);
        void setLevel(int level);
        void setClientFd(int clientFd);
        void setClientAddr(struct sockaddr_in clientAddr);

        // Client socket

        // Client channels
        void insertChannel(std::string const &name, Channel *channel);
        void eraseChannel(std::string const &name);
        void leaveChannel(std::string const &name);
        void leaveAllChannels();        
};
