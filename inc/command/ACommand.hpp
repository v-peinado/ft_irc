/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:02:28 by vpeinado          #+#    #+#             */
/*   Updated: 2025/05/08 18:47:39 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

#include "Server.hpp"

class ACommand
{
    protected:
    
        /*** Orthodox Canonical Form ***/
        
        ACommand();
        ACommand(const ACommand &src);
        ACommand &operator=(const ACommand &src);

        /*** SERVER ***/
        
        Server &_server;                            // Ref to server
        
    public:
    
        /*** Orthodox Canonical Form ***/
        
        ACommand(Server &server);
        virtual ~ACommand();

        /*** PURE VIRTUAL METHOD ***/
        
        virtual void run(std::vector<std::string> args, int fdClient) = 0;             
};

#endif