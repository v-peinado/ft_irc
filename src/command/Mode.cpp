#include "Mode.hpp"

/*****************************************************************************
 * ------------------------------- CONSTRUCTORS ---------------------------- *
 ****************************************************************************/

Mode::Mode(Server &server) : ACommand(server)
{
	// Constructor
}

Mode::~Mode()
{
	// Destructor
}

/******************************************************************************
 * ------------------------------- MEMBER FUNCTIONS ------------------------- *
 *****************************************************************************/

int Mode::validArgs(std::vector<std::string> args, int fdClient)
{
    
    std::string channelName = "";
	if (args.size() < 2)
	{
		this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
		return 0;
	}
	Channel *chan = this->_server.getChannelByName(args[1]);
    channelName = args[1];
	if (chan==NULL)
	{
		this->_server.sendError(403, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :No such channel\r\n");
		return 0;
	}
	if (args.size() == 3 && chan->GetAdmin(fdClient) == NULL)
	{
        this->_server.sendError(482, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :You're not channel operator\r\n");
		return 0;
	}
	return 1;
}

void changeInvite(Channel *channel, char oper)
{
	if (oper == '+')
		channel->SetInvitOnly(1);
	else if (oper == '-')
		channel->SetInvitOnly(0);
}
void changeTopic(Channel *channel, char oper)
{
	if (oper == '+')
		channel->SetTopicRestriction(1);
	else if (oper == '-')
		channel->SetTopicRestriction(0);
}

void Mode::changeKey(Channel *channel, char oper, std::vector<std::string> args, size_t *offset, int fdClient)
{
    std::string channelName = channel->GetChannelName();
	if (oper == '+')
	{
        if (args.size() < *offset + 4)
	    {
		    this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
		    return ;
	    }
		channel->SetHasKey(1);
		channel->SetKey(args[3 + *offset]);
		(*offset)++;
	}
	else if (oper == '-')
	{
		channel->SetHasKey(0);
		channel->SetKey("");
	}
}
void Mode::changeOper(Channel *channel, char oper, std::vector<std::string> args, size_t *offset, int fdClient)
{
    std::string channelName = channel->GetChannelName();
	if (args.size() < *offset + 4)
	{
		this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
		return ;
	}
    std::string name = args[3 + *offset];
    Client *cli = channel->GetClientByName(name);
    (*offset)++;
    if (cli == NULL)
    {
        this->_server.sendError(441, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :They aren't on that channel\r\n");
		return ;
    }
	if (oper == '+')
	{
        if (channel->GetAdmin(cli->getClientFd()) != NULL)
        {
            this->_server.sendError(400, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Already an operator\r\n");
		    return ;
        }
		else
        channel->changeClientToAdmin(name);
	}
	else if (oper == '-')
	{
        if (channel->GetAdmin(cli->getClientFd()) == NULL)
        {
            this->_server.sendError(400, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not an operator\r\n");
		    return ;
        }
		else
        channel->changeAdminToClient(name);
	}
}

void Mode::changeLimit(Channel *channel, char oper, std::vector<std::string> args, size_t *offset, int fdClient)
{
	if (oper == '+')
	{
        std::string channelName = channel->GetChannelName();
        if (args.size() < *offset + 4)
        {
            this->_server.sendError(461, this->_server.getUserByFd(fdClient)->getNickname(), channelName, fdClient, " :Not enough parameters\r\n");
            return ;
        }
        std::string num = args[3 + *offset];
        (*offset)++;
        channel->SetLimit(atoi(num.c_str()));
	}
	else if (oper == '-')
	{
        channel->SetLimit(0);
	}
}

void Mode::run(std::vector<std::string> args, int fdClient)
{
	char oper = '\0';

	if (validArgs(args, fdClient) == 0)
		return ;
	
	Channel *channel = this->_server.getChannelByName(args[1]);
	if (args.size() == 2)
	{
		std::string modes;
		std::string rply = "324 " + channel->GetChannelName();
		if (channel->GetTopicRest())
			modes += "t";
		if (channel->GetInviteOnly())
			modes += "i";
		if (channel->GetHasKey())
			modes += "k";
		if (channel->GetLimit() > 0)
			modes += "l";
		if (modes.size())
			rply += " +" + modes + "\r\n";
		send(fdClient, rply.c_str(), rply.size(), 0);
	}
	else
	{
		size_t offset = 0;
		std::string modeset = args[2];
		for(size_t i = 0; i < modeset.size(); i++)
		{
			if (modeset[i] == '+' || modeset[i] == '-')
				oper = modeset[i];
			else
			{
				if(modeset[i] == 'i')
					changeInvite(channel, oper);
				else if (modeset[i] == 't')
					changeTopic(channel, oper);
				else if (modeset[i] == 'k')
					changeKey(channel, oper, args, &offset, fdClient);
				else if (modeset[i] == 'o')
					changeOper(channel, oper, args, &offset, fdClient);
				else if (modeset[i] == 'l')
					changeLimit(channel, oper, args, &offset, fdClient);
                else if (modeset[i] == ' ')
                    continue ;
				else
					this->_server.sendError(472, this->_server.getUserByFd(fdClient)->getNickname(), modeset, fdClient, " :is unknwown mode char to me\r\n");
			}
		}
	}
}