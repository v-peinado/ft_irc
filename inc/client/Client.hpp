/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:27:27 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/16 17:08:37 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <map>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <exception>

class Client
{
    private:
        //Forma canonica no implementada
        
        Client(const Client &src);
        Client &operator=(const Client &src);

        // Client
        std::string _nickname;
        std::string _username;
        std::string _realname;
        bool _registered; // si su estado cambia por un comando de registro, o por un baneao, etc
        bool _active; // Si esta conectado o no, si pasa mucho tiempo sin actividad se desconecta o se le manda un ping, o se pone en su canal de bienvenida,etc
        int _level; // Nivel de permisos, si es admin, si es operador, si es usuario normal, etc

        // Client socket
        int _clientFd;
        std::string _clientIp;
        struct sockaddr_in _clientAddr; // Server address, arpa/inet.h
        struct pollfd _clientPollFd;
        
        // Maps
        //  std::map<std::string, Channel *> _channels; // Client channels

    public:
        // Forma canonica implementada
        Client();
        Client(struct sockaddr_in client, int fd);
        ~Client();

        // Getters, el poner referencias constantes o no, es para que no se pueda modificar el valor, dependera del desarrollo posterior
        std::string const &getNickname() const;
        std::string const &getUsername() const;
        std::string const &getRealname() const;
        bool const &getRegistered() const;
        bool const &getActive() const;
        int const &getLevel() const;
        //  std::map<std::string, Channel *> const &getChannels() const;
        //  Channel *getChannelByName(std::string const &name) const;
        int const &getClientFd() const;
        struct sockaddr_in &getClientAddr();
        std::string const &getClientIp() const;
        struct pollfd &getClientPollFd();
        
        // Setters
        void setNickname(std::string const &nickname);
        void setUsername(std::string const &username);
        void setRealname(std::string const &realname);
        void setRegistered(bool registered);
        void setActive(bool active);
        void setLevel(int level);
        void setClientFd(int clientFd);
        void setClientAddr(struct sockaddr_in clientAddr);
        void setClientIp(std::string clientIp);

        // Client socket

        // Client channels
        //  void insertChannel(std::string const &name, Channel *channel);
        void eraseChannel(std::string const &name);
        void leaveChannel(std::string const &name);
        void leaveAllChannels();        
};

#endif