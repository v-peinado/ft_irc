/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:13:05 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/20 14:43:32 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PASS_HPP
#define PASS_HPP

#include "ACommand.hpp"
#include "Server.hpp"



class Pass : public ACommand
{
    private:
        //Forma canonica no implementada
        Pass(const Pass &src);
        Pass &operator=(const Pass &src);
        Pass();
    public:
        Pass(Server &server);
        ~Pass();
        void run(std::vector<std::string> args, int fdClient);      
};

#endif