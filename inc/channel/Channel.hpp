/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ffons-ti <ffons-ti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 16:48:40 by ffons-ti          #+#    #+#             */
/*   Updated: 2024/09/25 16:37:17 by ffons-ti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <string>
#include <vector>

#include "Client.hpp"
#include "Server.hpp"

class Client;
class Channel
{
    private:
        int has_key;
        int invite_only;
        int limit;
        int topic_rest;
        std::string name;
        std::string key;
        std::string topic_name;
        std::vector<Client> clients;
	    std::vector<Client> admins;
    public:
        Channel();
        ~Channel();
        Channel(Channel const &src);
        Channel &operator=(Channel const &src);
        
        // Getters
        int GetHasKey();
        int GetInviteOnly();
        int GetLimit();
        int GetTopicRest();
        std::string GetChannelName();
        std::string GetKey();
        std::string GetTopicName();
        std::string ClientChannelList();
        Client *GetClient(int fd);
        Client *GetAdmin(int fd);
        
        // Setters
        void SetInvitOnly(int invit_only);
	    void SetHasKey(int key);
	    void SetLimit(int limt);
        void SetTopicRestriction(int value);
	    void SetTopicName(std::string topic_nam);
	    void SetKey(std::string password);
	    void SetName(std::string nam);

        // Methods
        void addClient(Client newClient);
        void addAdmin(Client newClient);
        void removeClient(int fd);
        void removeAdmin(int fd);
        bool changeClientToAdmin(std::string &nick);
        bool changeAdminToClient(std::string &nick);

        
        void sendToAll(std::string rply);
        
};

#endif