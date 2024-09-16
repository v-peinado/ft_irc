/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:10:55 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/16 17:55:25 by vpeinado         ###   ########.fr       */
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
    //try and catch
    Server server;
    server.startServer(argv[1], argv[2]);
    server.printServerInfo();
    server.runServer();

    return 0;
}