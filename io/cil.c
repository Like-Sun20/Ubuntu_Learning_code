#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);//创建套接字
    if( -1 == sockfd )
    {
        exit(1);
    }

    struct sockaddr_in saddr;//服务器的地址
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);//服务器端口
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");//服务器ip地址
    int res = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if( res == -1 )
    {
        printf("connect err\n");
        exit(1);
    }

    while( 1 )
    {
        printf("input\n");
        char buff[128] = {0};
        fgets(buff,128,stdin);
        if( strncmp(buff,"end",3) == 0)
        {
            break;
        }

        send(sockfd,buff,strlen(buff)-1,0);
        memset(buff,0,128);
        recv(sockfd,buff,127,0);
        printf("buff=%s\n",buff);
    }

    close(sockfd);
    exit(0);
}