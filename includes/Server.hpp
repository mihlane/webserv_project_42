#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>


#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include "Location.hpp"
#include "Client.hpp"
#include "ConfigAttr.hpp"




extern fd_set  readMaster;
extern fd_set  writeMaster;


class   Server
{
    private:

        int                                                     socket_listen;
        std::string                                             port;
        std::string                                             hostName;
        std::map<std::string, ConfigAttr>                       configAttrs;


    public:

        Server();
        Server(const Server &serv);
        Server  &operator= (const Server& serv);
        Server  &operator+= (const Server& serv);
        ~Server();

        void        openSocket();
        void        startListening();
        int         acceptClient(std::list<Client> &clients);
        void        clear();
        void        attributeExaminer();

        /*                              setters                                         */
        void        setHostName(std::vector<std::string> &tokens, unsigned int lineNumber);
        void        setPort(std::vector<std::string> &tokens, unsigned int lineNumber);
        void        setSocket(int socket_listen);
        void        addConfigAttr(const ConfigAttr &configAttr);

        /*                              getters                                         */
        const std::string                       &getHostName() const;
        const std::string                       &getPort() const;
        const int                               &getSocket() const;
        const std::map<std::string, ConfigAttr> &getConfigAttrs() const;

};



#endif