#include "server.h"

server::server()
{
    initialize();
}

server::~server()
{
    close(m_socket);
}

bool server::isID(std::string str)
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
    return false;
}

void server::initialize()
{
    //new socket
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(IP);
    if (bind(m_socket, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    if (listen(m_socket, 20) == -1)
    {
        perror("listen");
        exit(1);
    }
    len = sizeof(servaddr);
}

void server::getConn()
{
    while (1)
    {
        int conn = accept(m_socket, (struct sockaddr *)&servaddr, &len);
        acceptedSockets[conn] = 0;
        printf("accepted %d\n", conn);
    }
}

void server::getMsg()
{
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    while (1)
    {
        std::map<int, std::string>::iterator it;
        fd_set rset;
        int maxfd = 0;
        int ret = 0;
        //add all accepted sockets to rset;
        for(it = authSockets.begin(); it != authSockets.end(); ++it)
        {
            FD_ZERO(&rset);
            FD_SET(it->first, &rset);
            if (maxfd < it->first)
            {
                maxfd = it->first;
            }
        }

        ret = select(maxfd + 1, &rset, NULL, NULL, &tv);

        if (ret == -1)
        {
            std::cout << "Select error in authsocket()!" << std::endl;
            return;
        }
        else if (ret == 0)
        {
            continue;
        }
        else
        {
            for (it = authSockets.begin(); it != authSockets.end(); ++it)
            {
                if(FD_ISSET(it->first, &rset))
                {
                    memset(m_buf, 0, sizeof(m_buf));
                    int len = recv(it->first, m_buf, sizeof(m_buf), 0);

                    if(len == -1)
                    {
                        std::cout << "Client disconnected!" << std::endl;
                        msgs.erase(it->first);
                        it = authSockets.erase(it); //it pointed to the last of the deleted element
                        it--;
                        continue;
                    }

                    msgs[it->first] = std::string(m_buf);
                }

            }
        }

    }
}

/*
Iterate all the accepted sockets to get the user ID of them.
If got the user ID, insert it to authSockets.
*/

void server::authSocket()
{
    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    while (1)
    {
        std::map<int, int>::iterator it;
        fd_set rset;
        int maxfd = 0;
        int ret = 0;
        //add all accepted sockets to rset;
        for(it = acceptedSockets.begin(); it != acceptedSockets.end(); ++it)
        {
            FD_ZERO(&rset);
            FD_SET(it->first, &rset);
            if (maxfd < it->first)
            {
                maxfd = it->first;
            }
        }

        ret = select(maxfd + 1, &rset, NULL, NULL, &tv);

        if (ret == -1)
        {
            std::cout << "Select error in authsocket()!" << std::endl;
            return;
        }
        else if (ret == 0)
        {
            continue;
        }
        else
        {
            for (it = acceptedSockets.begin(); it != acceptedSockets.end(); ++it)
            {
                if(FD_ISSET(it->first, &rset))
                {
                    memset(m_buf, 0, sizeof(m_buf));
                    int len = recv(it->first, m_buf, sizeof(m_buf), 0);

                    if(len == -1)
                    {
                        std::cout << "Client disconnected!" << std::endl;
                        it = acceptedSockets.erase(it); //it pointed to the last of the deleted element
                        it--;
                        continue;
                    }

                    int idIndex = strspn(m_buf, "myIDWas:");
                    if (idIndex > 12)
                    {
                        char ID[1024] = {0};
                        std::string sbuf(m_buf);
                        std::string id = sbuf.substr(idIndex);

                        if (isID(id))
                        {

                            std::cout << "Found the id for socket " << it->first << std::endl;
                            authSockets[it->first] = id;
                            it = acceptedSockets.erase(it);
                            it--;
                        }
                    }
                }
                else
                {
                    it->second++;
                    if(it->second >= 10)
                    {
                        std::cout << "Out of times to get the id of socket: " << it->first << std::endl;
                        it = acceptedSockets.erase(it);
                        it--;
                    }
                }
            }
        }

    }
}

void server::broadcastFromServer()
{
    char buf[1024];
    while (1)
    {
        memset(buf, 0, 1024);
        fgets(buf, sizeof(buf), stdin);

        std::map<int, std::string>::iterator it;
        for (it = authSockets.begin(); it != authSockets.end(); ++it)
        {
            send(it->first, buf, sizeof(buf), 0);
        }
    }
}