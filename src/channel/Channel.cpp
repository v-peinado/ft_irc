/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 18:19:11 by ffons-ti          #+#    #+#             */
/*   Updated: 2024/10/16 13:56:57 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
	this->has_key = 0;
	this->invite_only = 0;
	this->limit = 0;
	this->topic_rest = 0;
	this->name = "";
	this->key = "";
	this->topic_name = "";
}

Channel::~Channel(){}

Channel::Channel(Channel const &src){*this = src;}

Channel &Channel::operator=(Channel const &src)
{
	if (this != &src)
	{
		this->invite_only = src.invite_only;
		this->has_key = src.has_key;
		this->limit = src.limit;
		this->topic_rest = src.topic_rest;
		this->name = src.name;
		this->key = src.key;
		this->topic_name = src.topic_name;
	}
	return *this;
}

int Channel::GetHasKey(){return this->has_key;}
int Channel::GetInviteOnly(){return this->invite_only;}
size_t Channel::GetLimit(){return this->limit;}
int Channel::GetTopicRest(){return this->topic_rest;}
std::string Channel::GetChannelName(){return this->name;}
std::string Channel::GetKey(){return this->key;}
std::string Channel::GetTopicName(){return this->topic_name;}
std::string Channel::ClientChannelList()
{
	std::string list;
	for(size_t i = 0; i < admins.size(); i++)
	{
		list += "@" + admins[i]->getNickname();
		if ((i + 1 ) < admins.size())
			list += " ";
	}
	if (clients.size())
		list += " ";
	for(size_t i = 0; i < clients.size(); i++)
	{
		list += clients[i]->getNickname();
		if ((i + 1) < clients.size())
			list += " ";
	}
	return list;
}

std::vector<Client *> Channel::GetClients(){return this->clients;}
std::vector<Client *> Channel::GetAdmins(){return this->admins;}
std::vector<int> Channel::GetInvitedClients(){return this->_invitedClients;}

Client *Channel::GetClient(int fd)
{
    for (std::vector<Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if ((*it)->getClientFd() == fd)
            return *it;
    }
    return NULL;
}

Client *Channel::GetClientByName(std::string nick)
{
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickname() == nick)
			return *it;
	}
	return NULL;
}

int Channel::GetClientFd(std::string nick)
{
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getNickname() == nick)
			return (*it)->getClientFd();
	}
	return -1;
}

Client *Channel::GetAdmin(int fd)
{
for(std::vector<Client *>::iterator it = admins.begin(); it != admins.end(); it++)
	{
        if ((*it)->getClientFd() == fd)
            return *it;
    }
    return NULL;
}

void Channel::SetInvitOnly(int invit_only){this->invite_only = invit_only;}
void Channel::SetHasKey(int key){this->has_key = key;}
void Channel::SetLimit(int limt){this->limit = limt;}
void Channel::SetTopicRestriction(int value){this->topic_rest = value;}
void Channel::SetTopicName(std::string topic_nam){this->topic_name = topic_nam;}
void Channel::SetKey(std::string password){this->key = password;}
void Channel::SetName(std::string nam){this->name = nam;}

void Channel::addClient (Client *newClient){clients.push_back(newClient);}
void Channel::addAdmin (Client *newClient){admins.push_back(newClient);}
void Channel::addInvitedClients(int fd){_invitedClients.push_back(fd);}
void Channel::removeClient (int fd)
{
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it){
		if ((*it)->getClientFd() == fd)
			{clients.erase(it); break;}
	}
}
void Channel::removeAdmin (int fd)
{
    for (std::vector<Client *>::iterator it = admins.begin(); it != clients.end(); ++it){
		if ((*it)->getClientFd() == fd)
			{admins.erase(it); break;}
	}
}

bool Channel::changeClientToAdmin(std::string &nick)
{
    size_t i = 0;
	for(; i < clients.size(); i++){
		if(clients[i]->getNickname() == nick)
			break;
	}
	if(i < clients.size()){
		admins.push_back(clients[i]);
		clients.erase(i + clients.begin());
		return true;
	}
	return false;
}

bool Channel::changeAdminToClient(std::string &nick)
{
    size_t i = 0;
	for(; i < admins.size(); i++){
		if(admins[i]->getNickname() == nick)
			break;
	}
	if(i < admins.size()){
		clients.push_back(admins[i]);
		admins.erase(i + admins.begin());
		return true;
	}
	return false;
}

void Channel::sendToAll(std::string rply)
{
	for(size_t i = 0; i < clients.size(); i++)
		if(send(clients[i]->getClientFd(), rply.c_str(), rply.size(),0) == -1)
			std::cerr << "send() faild" << std::endl;
}

bool Channel::isClientInvited(int fd)
{
	for (std::vector<int>::iterator it = _invitedClients.begin(); it != _invitedClients.end(); ++it)
	{
		if (*it == fd)
			return true;
	}
	return false;
}

bool Channel::isClientAdmin(int fd)
{
	for (std::vector<Client *>::iterator it = admins.begin(); it != admins.end(); ++it)
	{
		if ((*it)->getClientFd() == fd)
			return true;
	}
	return false;
}

bool Channel::isClientInChannel(int fd)
{
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if ((*it)->getClientFd() == fd)
			return true;
	}
	return false;
}

void Channel::removeInvitedClient(int fd)
{
	for (std::vector<int>::iterator it = _invitedClients.begin(); it != _invitedClients.end(); ++it)
	{
		if (*it == fd)
		{
			_invitedClients.erase(it);
			break;
		}
	}
}
