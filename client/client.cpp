#include "client.h"

void Client::Error(const char* message)
{
    printf(message,strerror(errno),errno);
    exit(0);
}

void Client::Socket()
{
     if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //初始化套接字，AF_INET表示协议族，SOCK_STREAM是采用TCP协议
        Error("create socket error: %s(errno: %d)\n");// 返回值小于0，报错

    memset(&servaddr, 0, sizeof(servaddr));	//将servaddr结构体的内容置0
    servaddr.sin_family = AF_INET;	//规定协议族为SOCK_STREAM
    servaddr.sin_port = htons(port);//绑定端口
}

void Client::Connet()
{
    printf("ip:%s\n",ip);
     if( inet_pton(AF_INET, ip, &servaddr.sin_addr) <= 0) //ip转化函数，将ip转化后赋值给servaddr.sin_addr
    {
        printf("inet_pton error for %s\n",ip);
        exit(0);
    }
    printf("port:%d\n",port);
     if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)	//用connet函数进行连接
        Error("connect error: %s(errno: %d)\n");
}

void Client::handle()
{
    printf("send msg to server: \n");
    fgets(sendline, MAXLINE, stdin);//从屏幕输入
    if( send(sockfd, sendline, strlen(sendline), 0) < 0)	//使用send发送
        Error("send msg error: %s(errno: %d)\n");
}

void Client::start()
{
    Socket();
    Connet();
}