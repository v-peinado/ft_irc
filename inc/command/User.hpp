/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:03:34 by vpeinado          #+#    #+#             */
/*   Updated: 2025/05/08 19:02:00 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class User : public ACommand
{
    private:
    
        /*** Orthodox Canonical Form (NOT IMPLEMENTED) ***/
        
        User(const User &src);
        User &operator=(const User &src);
        User();
        
    public:
    
        /*** Orthodox Canonical Form ***/
        
        User(Server &server);
        ~User();

        /*** MEMBER FUNCTIONS ***/
        
        int validArgs(std::vector<std::string> args);
        void run(std::vector<std::string> args, int fdClient);      
};

#endif