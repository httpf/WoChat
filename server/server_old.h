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

class SockServer
{
private:
    static int m_socket;
    static struct sockaddr_in servaddr;

    std::map<int,int> acceptedSockets; //sock id -> trying times
    std::map<int, std::string> authSockets; //socket id -> user id

    char m_buf[1024];
    std::map<int, std::string> msgs;
    static bool initialized;


public:
    SockServer();
    ~SockServer();

    static void initialize();

    void getConn();
    void getMsg();
    void authSocket();  //authenticate socket by checking user ID
    void broadcastFromServer(); //brocadcast message to every auth socket
};

#endif