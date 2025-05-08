/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:01:42 by vpeinado          #+#    #+#             */
/*   Updated: 2025/05/08 18:55:19 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Invite_HPP
#define Invite_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class Invite : public ACommand
{
    private:

        /*** Orthodox Canonical Form (NOT IMPLEMENTED)***/
        
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