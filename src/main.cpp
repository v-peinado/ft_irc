/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:10:55 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/21 13:47:58 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"

int parseArgs(char *port, char *password)
{
    int portInt = atoi(port);
    if (portInt < 1024 || portInt > 65536)
    {
        std::cerr << "Port must be between 1024 and 65536" << std::endl;
        return 0;
    }
    if (strlen(password) < 8 || strlen(password) > 16)
    {
        std::cerr << "Password must be between 8 and 16 characters long" << std::endl;
        return 0;
    }
    return 1;
}

int main(int argc, char **argv)
{  
    if (argc == 3 && parseArgs(argv[1], argv[2]) == 0)
    {   
        std::cerr << "Usage: ./ircserv [port] [password]" << std::endl; return 1;
    }
    try
    {
        Server server(argv[1], argv[2]);
        server.startServer();
        server.printServerInfo();
        server.runServer();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}