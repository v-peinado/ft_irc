/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vpeinado <victor@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:57:46 by vpeinado          #+#    #+#             */
/*   Updated: 2024/10/18 16:56:53 by vpeinado         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Join.hpp"

/*****************************************************************************
 * ------------------------------- CONSTRUCTORS ---------------------------- *
 ****************************************************************************/

Join::Join(Server &server) : ACommand(server)
{
    // Constructor
}

Join::~Join()
{
    // Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

int Join::validArgs(std::vector<std::string> args, int fdClient)
{
    if (this->_server.getUserByFd(fdClient)->getNickname() == "" 
        || this->_server.getUserByFd(fdClient)->getUsername() == "" 
        || this->_server.getUserByFd(fdClient)->getRealname() == "")
    {
        std::string channelName = ""; 
        std::string nickName = "";
        this->_server.sendError(451, nickName, channelName, fdClient, " :You have not registered\r\n");
        return 0;
    }
    std::string channelName = args.size() > 1 ? args[1] : "";     //args[1] es el nombre del canal
    if (args.size() < 2 || args.size() > 3 || args[1].empty())
    {
        //<client> <command> :Not enough parameters
        this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
        return 0;
    }
    return 1;
}

void Join::sendJoinMessages(int fdClient, const std::string& channelName, Channel* channel) {
    // Obtener el cliente que se ha unido
    Client* client = this->_server.getUserByFd(fdClient);

    // Mensaje de JOIN que se enviará a todos en el canal
    std::string joinMsg = ":" + client->getHostName() + "@" + client->getClientIp() +
                          " JOIN " + channelName + "\r\n";
    channel->sendToAll(joinMsg); // Enviar el mensaje a todos en el canal

    // Respuesta con la lista de usuarios en el canal (mensaje 353)
    std::string rply = ": 353 " + client->getNickname() + " @ " + channelName + 
                       " :" + channel->ClientChannelList() + "\r\n";
    this->_server.sendResponse(rply, fdClient);

    // Mensaje de fin de la lista de nombres (mensaje 366)
    rply = ": 366 " + client->getNickname() + " " + channelName + " :End of /NAMES list\r\n";
    this->_server.sendResponse(rply, fdClient);

    // Enviar el tema del canal (mensaje 332)
    std::string topic = channel->GetTopicName().empty() ? "No topic is set" : channel->GetTopicName();
    rply = ": 332 " + client->getNickname() + " " + channelName + " :" + topic + "\r\n";
    this->_server.sendResponse(rply, fdClient);
}

void Join::run(std::vector<std::string> args, int fdClient)
{
        if (this->validArgs(args, fdClient) == 0)
            return;
        std::string channelName = args[1][0] == '#' ? args[1] : "#" + args[1];  //solo se añade el # si no lo tiene, irssi lo hace automaticamente
        std::string channelKey = args.size() == 3 ? args[2] : ""; //quiza no se permitA el condicional ternario
        std::map<std::string, Channel *> &channels = this->_server.getChannels();
        std::map<std::string, Channel *>::iterator it = channels.find(channelName);
        if (it == channels.end())         //si no existe el canal, se crea
        {
            // Creamos nuevo canal y seteamos sus atributos
            Channel *newChannel = new Channel();        
            newChannel->SetName(channelName);
            newChannel->SetKey(channelKey);
            
            // Si la clave del canal está vacía, se setea a 0, de lo contrario a 1
            if (channelKey == "")
                newChannel->SetHasKey(0);
            else
                newChannel->SetHasKey(1);
            channels.insert(std::pair<std::string, Channel *>(channelName, newChannel));
            newChannel->addClient(this->_server.getUserByFd(fdClient));
            newChannel->addAdmin(this->_server.getUserByFd(fdClient));
            // Informar a todos en el canal
            this->sendJoinMessages(fdClient, channelName, newChannel);
        }
        if (it != channels.end())  // no se llega al final del map, por lo que el canal ya existe
        {
            // Verificar si el usuario ya está en el canal
            if (it->second->GetClient(fdClient) != NULL)
            {
                std::cout << this->_server.getUserByFd(fdClient)->getNickname() << " is already on channel " << channelName << std::endl;
                //this->_server.sendError(443, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :is already on channel\r\n");
                return;
            }
            // Verificar si el canal tiene clave y si la clave es correcta
            if (it->second->GetHasKey() == 1 && it->second->GetKey() != channelKey)
            {
                this->_server.sendError(475, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Cannot join channel (+k) - bad key\r\n");
                return;
            }

            // Verificar si el canal está lleno
            if (it->second->GetLimit() != 0 && it->second->GetLimit() == it->second->GetClients().size())
            {
                this->_server.sendError(471, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Cannot join channel (+l)\r\n");
                return;
            }
            // Verificar si el canal es solo para invitados
            if (it->second->GetInviteOnly() == 1 && !it->second->isClientInvited(fdClient))
            {
                this->_server.sendError(473, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Cannot join channel (+i)\r\n");
                return;
            }
            // Si es invitado, agregarlo al canal
            it->second->addClient(this->_server.getUserByFd(fdClient));
            std::cout << this->_server.getUserByFd(fdClient)->getNickname() << " has joined the channel " << channelName << std::endl;

            // Informar a todos en el canal
            this->sendJoinMessages(fdClient, channelName, it->second);
        }            
}
