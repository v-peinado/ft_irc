/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 13:27:36 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/30 13:28:28 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KICK_HPP
#define KICK_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class Kick : public ACommand
{
    private:

        /*** Orthodox Canonical Form ***/
        
        Kick(const Kick &src);
        Kick &operator=(const Kick &src);
        Kick();
        
    public:
        
        /*** Orthodox Canonical Form ***/
        
        Kick(Server &server);
        ~Kick();

        /*** MEMBER FUNCTIONS ***/
        
        int validArgs(std::vector<std::string> args, int fdClient);
        void run(std::vector<std::string> args, int fdClient);  
            
};

#endif