/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:01:42 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/01 13:02:23 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Invite_HPP
#define Invite_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class Invite : public ACommand
{
    private:

        /*** Orthodox Canonical Form ***/
        
        Invite(const Invite &src);
        Invite &operator=(const Invite &src);
        Invite();
        
    public:
        
        /*** Orthodox Canonical Form ***/
        
        Invite(Server &server);
        ~Invite();

        /*** MEMBER FUNCTIONS ***/
        
        int validArgs(std::vector<std::string> args, int fdClient);
        void run(std::vector<std::string> args, int fdClient);  
            
};

#endif