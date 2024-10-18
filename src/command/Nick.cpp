/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:39:42 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/18 12:08:44 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Nick.hpp"

/*****************************************************************************
 * ------------------------------- CONSTRUCTORS ---------------------------- *
 ****************************************************************************/

Nick::Nick(Server &server) : ACommand(server)
{
    // Constructor
}

Nick::~Nick()
{
    // Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

int Nick::isValidNickname(std::string& nickname)
{
		
	if(!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#'))
		return 0;
	for(size_t i = 1; i < nickname.size(); i++)
	{
		if(!std::isalnum(nickname[i]) && nickname[i] != '_')
			return 0;
	}
	return 1;
}

int Nick::validArgs(std::vector<std::string> args, int fdClient)
{
    std::string channelName = ""; 
    std::string nickName;
    if (args.size() < 2)
    {
        nickName = "";
        this->_server.sendError(431, nickName, channelName, fdClient, " :No nickname given\r\n");
        return 0;
    }
   
    if (!isValidNickname(args[1]))
    {
        nickName = args[1];
        this->_server.sendError(432, nickName, channelName, fdClient, " :Erroneus nickname\r\n");
        return 0;
    }
    
    std::map<int, Client *> users = this->_server.getUsers();
    for (std::map<int, Client *>::iterator it = users.begin(); it != users.end(); it++)       //comprobar si el nickname ya esta en uso
    {
        if (it->second->getNickname() == args[1])
        {
            nickName = args[1];
            this->_server.sendError(433, nickName, channelName, fdClient, " :Nickname is already in use\r\n");
            return 0;
        }
    }
    return 1;
}

void Nick::run(std::vector<std::string> args, int fdClient)
{
    if (validArgs(args, fdClient))
    {
        if (this->_server.getUserByFd(fdClient)->getRegistered() == 0)
        {
            std::string channelName = ""; 
            std::string nickName = "";
            this->_server.sendError(451, nickName, channelName, fdClient, " :You have not registered\r\n");
            return;
        }
        this->_server.getUserByFd(fdClient)->setNickname(args[1]);
    }
    else
    {
        return;
    }
}