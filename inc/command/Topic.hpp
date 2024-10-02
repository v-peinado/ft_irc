/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:17:14 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/02 20:17:22 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Topic_HPP
#define Topic_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class Topic : public ACommand
{
    private:

        /*** Orthodox Canonical Form ***/
        
        Topic(const Topic &src);
        Topic &operator=(const Topic &src);
        Topic();
        
    public:
        
        /*** Orthodox Canonical Form ***/
        
        Topic(Server &server);
        ~Topic();

        /*** MEMBER FUNCTIONS ***/
        
        //int validArgs(std::vector<std::string> args, int fdClient);
        //void run(std::vector<std::string> args, int fdClient);  
            
};

#endif