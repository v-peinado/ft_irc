/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:16:36 by vpeinado          #+#    #+#             */
/*   Updated: 2025/05/08 22:19:40 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ACommand.hpp"
#include "Server.hpp"

ACommand::ACommand(Server &server) : _server(server) // Constructor, will receive a reference to the server
{}

ACommand::~ACommand()
{
    // Destructor
}

