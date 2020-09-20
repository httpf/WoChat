#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>
#include <thread>
#include <list>
#include <map>
#include <string>

#define PORT 7654
#define IP "127.0.0.1"

class server
{
private:
    int m_socket;
    struct sockaddr_in servaddr;
    socklen_t len;
    std::map<int,int> acceptedSockets; //sock id -> trying times
    std::map<int, std::string> authSockets; //socket id -> user id

    char m_buf[1024];
    std::map<int, std::string> msgs;

    bool isID(std::string str);
    void initialize();

public:
    server();
    ~server();

    void getConn();
    void getMsg();
    void authSocket();  //authenticate socket by checking user ID
    void broadcastFromServer(); //brocadcast message to every auth socket
};

#endif