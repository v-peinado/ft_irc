/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:16:36 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/19 20:40:46 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "ACommand.hpp"

ACommand::ACommand(Server &server) : _server(server)
{
    // Constructor
}
ACommand::~ACommand()
{
    // Destructor
}

