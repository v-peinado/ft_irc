/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:13:05 by vpeinado          #+#    #+#             */
/*   Updated: 2025/05/08 18:59:21 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PASS_HPP
#define PASS_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class Pass : public ACommand
{
    private:
        
        /*** Orthodox Canonical Form (NOT IMPLEMENTED) ***/
        
        Pass(const Pass &src);
        Pass &operator=(const Pass &src);
        Pass();
        
    public:
    
        /*** Orthodox Canonical Form ***/
        
        Pass(Server &server);
        ~Pass();

        /*** MEMBER FUNCTIONS ***/
        
        void run(std::vector<std::string> args, int fdClient);  
            
};

#endif