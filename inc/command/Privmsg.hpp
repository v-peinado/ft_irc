/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 14:01:02 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/03 22:04:27 by vpeinado         ###   ########.fr       */
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
        
        bool userInChannel(Channel *chan, std::string user);
        std::vector<std::string> splitTarget(std::string str);
        int validArgs(std::vector<std::string> args, int fdClient);
        void run(std::vector<std::string> args, int fdClient);  
            
};

#endif