/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:27:27 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/13 17:14:32 by vpeinado         ###   ########.fr       */
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
        
};
