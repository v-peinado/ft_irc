/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 19:51:25 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/17 14:22:06 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"
#include "Server.hpp"

class Join : public ACommand
{
    private:
    
        /*** Orthodox Canonical Form ***/
        
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