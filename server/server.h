#ifndef SERVER_H
#define SERVER_H

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

const int MAXLINE = 100;

class Server
{
    private:
        int listenfd;
        int connfd;
        struct sockaddr_in servaddr;
        char *buf;
        int port;
        int maxsize;

        void error(const char *);

    public:
        Server(int po = 7890, int size = 50): port(po), maxsize(size)
        { 
            buf = new char[MAXLINE];
        }

        void socket();
        void bind();
        void listen();
        void handle();
        void start();
        void stop()
        {
            close(listenfd);
        }

        ~Server()
        {
            delete [] buf;
        }

};

#endif