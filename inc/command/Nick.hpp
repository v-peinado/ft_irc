/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:26:37 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/20 17:28:43 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_HPP
#define NICK_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class Nick : public ACommand
{
    private:
        //Forma canonica no implementada
        Nick(const Nick &src);
        Nick &operator=(const Nick &src);
        Nick();
    public:
        Nick(Server &server);
        ~Nick();
        int validArgs(std::vector<std::string> args);
        int searchNick(std::string nick, int fd);
        void run(std::vector<std::string> args, int fdClient);      
};

#endif