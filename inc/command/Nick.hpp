/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 17:26:37 by vpeinado          #+#    #+#             */
/*   Updated: 2024/09/22 17:04:22 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NICK_HPP
#define NICK_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class Nick : public ACommand
{
    private:
        /**  */
        Nick(const Nick &src);
        Nick &operator=(const Nick &src);
        Nick();
    public:
        Nick(Server &server);
        ~Nick();
        int validArgs(std::vector<std::string> args, int fdClient);
        void run(std::vector<std::string> args, int fdClient);      
};

#endif