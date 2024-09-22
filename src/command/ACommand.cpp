/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:16:36 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/22 17:02:16 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ACommand.hpp"
#include "Server.hpp"

ACommand::ACommand(Server &server) : _server(server) // Constructor, recibira una referencia al servidor
{}

ACommand::~ACommand()
{
    // Destructor
}

