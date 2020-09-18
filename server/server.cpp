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

int s;
struct sockaddr_in servaddr;
socklen_t len;
std::list<int> li;
std::map<std::string, int> client;

bool isNum(std::string str)
{
    for (int i = 0; i < str.size(); i++)
    {
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57)
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}

void getConn()
{
    while (1)
    {
        int conn = accept(s, (struct sockaddr *)&servaddr, &len);
        li.push_back(conn);
        printf("%d\n", conn);
    }
}

void getID()
{
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    while (1)
    {
        std::list<int>::iterator it;
        for (it = li.begin(); it != li.end(); ++it)
        {
            fd_set rfds;
            FD_ZERO(&rfds);
            int maxfd = 0;
            int retval = 0;
            FD_SET(*it, &rfds);
            if (maxfd < *it)
            {
                maxfd = *it;
            }
            retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
            if (retval == -1)
            {
                printf("select error\n");
            }
            else if (retval == 0)
            {
                //printf("not message\n");
            }
            else
            {
                char buf[1024];
                memset(buf, 0, sizeof(buf));
                int len = recv(*it, buf, sizeof(buf), 0);

                int idIndex = strspn(buf, "myIDWas:");
                if (idIndex > 12)
                {
                    char ID[1024] = {0};
                    std::string sbuf(buf);
                    std::string id = sbuf.substr(idIndex);

                    if (isNum(id))
                    {
                        client[id] = *it;
                        li.erase(it);
                        std::cout << "New connected id: " << id << std::endl;
                    }
                }
            }
        }
        sleep(1);
    }
}

void getData()
{
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    while (1)
    {
        std::map<std::string, int>::iterator it;
        for (it = client.begin(); it != client.end(); ++it)
        {
            fd_set rfds;
            FD_ZERO(&rfds);
            int maxfd = 0;
            int retval = 0;
            FD_SET(it->second, &rfds);
            if (maxfd < it->second)
            {
                maxfd = it->second;
            }
            retval = select(maxfd + 1, &rfds, NULL, NULL, &tv);
            if (retval == -1)
            {
                printf("select error\n");
            }
            else if (retval == 0)
            {
            }
            else
            {
                char buf[1024];
                memset(buf, 0, sizeof(buf));
                int len = recv(it->second, buf, sizeof(buf), 0);
                if(len > 0)
                {
                    printf("Message: %s\n", buf);
                }
                else if (len == 0)
                {
                    printf("%s closed!\n", it->first.c_str());
                    client.erase(it);
                }
                else
                {
                    printf("%s socket error!\n", it->first.c_str());
                    client.erase(it);
                }

            }
        }
        sleep(1);
    }
}

void broadcastFromServer()
{
    char buf[1024];
    while (1)
    {
        memset(buf, 0, 1024);
        fgets(buf, sizeof(buf), stdin);

        std::map<std::string, int>::iterator it;
        for (it = client.begin(); it != client.end(); ++it)
        {
            send(it->second, buf, sizeof(buf), 0);
        }
    }
}

int main()
{
    //new socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(IP);
    if (bind(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    if (listen(s, 20) == -1)
    {
        perror("listen");
        exit(1);
    }
    len = sizeof(servaddr);

    std::thread t(getConn);
    t.detach();

    std::thread t2(getID);
    t2.detach();
    while (1)
    {
    }

    close(s);
    return 0;
}