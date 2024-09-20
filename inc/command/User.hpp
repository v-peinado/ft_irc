/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:03:34 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/20 16:45:53 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class User : public ACommand
{
    private:
        //Forma canonica no implementada
        User(const User &src);
        User &operator=(const User &src);
        User();
    public:
        User(Server &server);
        ~User();
        int validArgs(std::vector<std::string> args);
        void run(std::vector<std::string> args, int fdClient);      
};

#endif