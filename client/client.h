#ifndef CLIENT_H
#define CLIENT_H
#include "../common/unified.h"

class Client
{
private:
    int sockfd;
    char *sendline; //发送的内容
    char *ip;
    int port;
    struct sockaddr_in servaddr;

    void Error(const char *);

public:
    Client(char *IP, int Po = 8000) : ip(IP), port(Po)
    {
        sendline = new char[MAXLINE];
    }

    void Socket(); //建立socket
    void Connet(); //连接
    void handle(); //处理
    void start();  //启动
    void stop() { close(sockfd); }

    ~Client()
    {
        delete[] sendline;
    }
};
#endif