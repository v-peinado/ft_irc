/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:26:37 by vpeinado          #+#    #+#             */
/*   Updated: 2025/05/08 18:57:15 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_HPP
#define NICK_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class Nick : public ACommand
{
    private:

        /*** Orthodox Canonical Form (NOT IMPLEMENTED) ***/
        
        Nick(const Nick &src);
        Nick &operator=(const Nick &src);
        Nick();
        
    public:
        
        /*** Orthodox Canonical Form ***/
        
        Nick(Server &server);
        ~Nick();

        /*** MEMBER FUNCTIONS ***/
        
        int validArgs(std::vector<std::string> args, int fdClient);
        void run(std::vector<std::string> args, int fdClient); 
        int isValidNickname(std::string& nickname);
            
};

#endif