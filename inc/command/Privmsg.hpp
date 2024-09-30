/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:01:02 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/30 11:23:21 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVMSG_HPP
#define PRIVMSG_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class Privmsg : public ACommand
{
    private:

        /*** Orthodox Canonical Form ***/
        
        Privmsg(const Privmsg &src);
        Privmsg &operator=(const Privmsg &src);
        Privmsg();
        
    public:
        
        /*** Orthodox Canonical Form ***/
        
        Privmsg(Server &server);
        ~Privmsg();

        /*** MEMBER FUNCTIONS ***/
        
        int validArgs(std::vector<std::string> args, int fdClient);
        void run(std::vector<std::string> args, int fdClient);  
            
};

#endif