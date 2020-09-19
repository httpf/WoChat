#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

class server
{
private:
    int socket;
    struct sockaddr_in servaddr;
public:
    server();
    ~server();
};

#endif