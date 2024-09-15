/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:10:55 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/15 13:24:04 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"

int main(int argc, char **argv)
{
    // No analizamos los argumentos, solo comprobamos que haya 3
    
    if (argc != 3)
    {   
        std::cerr << "Usage: ./ircserv [port] [password]" << std::endl; return 1;
    }
    Server server;
    server.startServer(argv[1], argv[2]);
    std::cout << "Server started on port " << server.getPort() << std::endl;
    std::cout << "Password: " << server.getPassword() << std::endl;
    std::cout << "ADress: " << server.getServerAddr().sin_addr.s_addr << std::endl;
    std::cout << "Port: " << ntohs(server.getServerAddr().sin_port) << std::endl;
    std::cout << "Server fd: " << server.getServerFd() << std::endl;
    return 0;
}