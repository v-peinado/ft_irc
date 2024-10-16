#ifndef MODE_HPP
#define MODE_HPP

#include "ACommand.hpp"
#include "Server.hpp"

class Mode : public ACommand
{
    private:
        //Forma canonica no implementada
        Mode(const Mode &src);
        Mode &operator=(const Mode &src);
        Mode();
    public:
        Mode(Server &server);
        ~Mode();
        int validArgs(std::vector<std::string> args, int fdClient);
        void run(std::vector<std::string> args, int fdClient);
        void changeLimit(Channel *channel, char oper, std::vector<std::string> args, size_t *offset, int fdClient);
        void changeOper(Channel *channel, char oper, std::vector<std::string> args, size_t *offset, int fdClient);
        void changeKey(Channel *channel, char oper, std::vector<std::string> args, size_t *offset, int fdClient);
};

#endif