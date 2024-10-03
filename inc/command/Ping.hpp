/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 18:49:57 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/03 18:52:19 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_HPP
#define PING_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class Ping : public ACommand
{
    private:
        
        /*** Orthodox Canonical Form ***/
        
        Ping(const Ping &src);
        Ping &operator=(const Ping &src);
        Ping();
        
    public:
    
        /*** Orthodox Canonical Form ***/
        
        Ping(Server &server);
        ~Ping();

        /*** MEMBER FUNCTIONS ***/
        
        int validArgs(std::vector<std::string> args);
        void run(std::vector<std::string> args, int fdClient);  
            
};

#endif