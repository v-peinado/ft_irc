/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:27:27 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/05 19:13:35 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <exception>

class Client
{
    private:
    
        /*** Orthodox Canonical Form ***/
        
        // Client(const Client &src);
        // Client &operator=(const Client &src);

        /*** CLIENT ***/
        
        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string buffer;
        bool _registered;

        /*** SOCKET ***/
        
        int _clientFd;
        std::string _clientIp;
        struct sockaddr_in _clientAddr;
        struct pollfd _clientPollFd;

        /*** CHANNELS ***/

        std::vector<std::string> _channels;          //nueva

    public:
    
        /*** Orthodox Canonical Form ***/
        
        Client();
        ~Client();
        Client(const Client &src);
        Client &operator=(const Client &src);

        /*** GETTERS ***/
        
        std::string const &getNickname() const;
        std::string const &getUsername() const;
        std::string const &getRealname() const;
        bool const &getRegistered() const;
        int const &getClientFd() const;
        struct sockaddr_in &getClientAddr();
        std::string const &getClientIp() const;
        struct pollfd &getClientPollFd();
        std::string &getBuffer();
        std::string getHostName();
        
        /*** SETTERS ***/
        
        void setNickname(std::string const &nickname);
        void setUsername(std::string const &username);
        void setRealname(std::string const &realname);
        void setRegistered(bool registered);
        void setClientFd(int clientFd);
        void setClientAddr(struct sockaddr_in clientAddr);
        void setClientIp(std::string clientIp);
        void setBuffer(std::string buffer);

        /*** MEMBER FUNCTIONS ***/
        
        void clearBuffer();

        /*** CHANNELS ***/

        void deleteFromAllChannels(std::string channelName);            //nueva
        //std::vector<std::string> getChannels();                     //nueva
              
};

#endif