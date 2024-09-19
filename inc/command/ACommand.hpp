/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 19:02:28 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/19 14:03:14 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACOMMAND_HPP
#define ACOMMAND_HPP

class ACommand
{
    protected:
        //Forma canonica no implementada
        ACommand(const ACommand &src);
        ACommand &operator=(const ACommand &src);
        
    public:
        ACommand();
        virtual ~ACommand();
        virtual void run() = 0;       
};

#endif