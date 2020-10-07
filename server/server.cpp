#include "server.h"

void Server::Error(const char* message)
{
    printf(message,strerror(errno),errno);
    exit(0);
}

void Server::Socket()
{
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )	//初始化套接字，AF_INET表示协议族，SOCK_STREAM是采用TCP协议
        Error("create socket error: %s(errno: %d)\n");// 返回值==-1，报错

    memset(&servaddr, 0, sizeof(servaddr));//将servaddr结构体的内容置0
    servaddr.sin_family = AF_INET;//规定协议族为SOCK_STREAM
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//这里表示的是需要监听的IP地址，也就是接受哪些IP发过来的数据包，INADDR_ANY任意，即0.0.0.0  如果是本地通信，那么就是127.0.0.1
    servaddr.sin_port = htons(port); //绑定端口
}

void Server::Bind()
{
     if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) //参数分别是之前的文件描述符，网络通信地址及其大小
            Error("bind socket error: %s(errno: %d)\n");
}

void Server::Listen()
{

    if( listen(listenfd, maxsize) == -1)//文件描述符以及最大连接数
         Error("listen socket error: %s(errno: %d)\n");
}

void Server::Handle()
{
     printf("======waiting for client's request======\n");
    while(1){
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1){	//accept用来接受端口发来的数据，connfd文件描述符
            printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
            continue;
        }
        int n = recv(connfd, buff, MAXLINE, 0);	//将接收到的数据放入buff中
        buff[n] = '\0';
        printf("recv msg from client: %s\n", buff);
        close(connfd);
    }
}

void Server::start()
{
    Socket();
    Bind();
    Listen();
}