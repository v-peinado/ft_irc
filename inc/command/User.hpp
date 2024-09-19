/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 19:09:42 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/19 19:11:07 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

class User : public ACommand
{
    private:
        //Forma canonica no implementada
        User(const User &src);
        User &operator=(const User &src);
        
    public:
        User();
        ~User();
        void run();       
};