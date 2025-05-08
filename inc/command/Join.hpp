/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 19:51:25 by vpeinado          #+#    #+#             */
/*   Updated: 2025/05/08 18:55:41 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"
#include "Server.hpp"

class Join : public ACommand
{
    private:
    
        /*** Orthodox Canonical Form (NOT IMPLEMENTED) ***/
        
        Join(const Join &src);
        Join &operator=(const Join &src);
        Join();
        
    public:
        
        /*** Orthodox Canonical Form ***/
        
        Join(Server &server);
        ~Join();

        /*** MEMBER FUNCTIONS ***/
        
        int validArgs(std::vector<std::string> args, int fdClient);
        void run(std::vector<std::string> args, int fdClient);
        void sendJoinMessages(int fdClient, const std::string& channelName, Channel* channel);
    
};