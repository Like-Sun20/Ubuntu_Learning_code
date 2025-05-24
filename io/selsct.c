#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define  MAXFD  10

int socket_init();

void fds_init(int fds[])
{
    for(int i = 0; i < MAXFD; i++)
    {
        fds[i] = -1;
    }
}
void fds_add(int fd, int fds[])
{
    for(int i = 0; i < MAXFD; i++)
    {
        if( -1 == fds[i] )
        {
            fds[i] = fd;
            break;//！！！
        }
    }
}

void fds_del(int fd, int fds[])
{
    for(int i = 0; i < MAXFD; i++)
    {
        if( fds[i] == fd )
        {
            fds[i] = -1;
            break;
        }
    }
}

void accept_client(int sockfd, int fds[])
{
    int c = accept(sockfd,NULL,NULL);
    if( c < 0 )
    {
        return ;
    }

    printf("accept c=%d\n",c);
    fds_add(c,fds);

}
void recv_data(int c, int fds[])
{
    char buff[128] = {0};
    int n = recv(c,buff,127,0);
    if( n <= 0 )
    {
        close(c);
        fds_del(c,fds);
        printf("client close(%d)\n",c);
        return;
    }

    printf("buff(%d)=%s\n",c,buff);
    send(c,"ok",2,0);
}
int main()
{
    int sockfd = socket_init();
    if( -1 == sockfd )
    {
        exit(1);
    }

    int fds[MAXFD];//收集描述符
    fds_init(fds);//清空
    fds_add(sockfd,fds);//将监听套接字添加到数组中,

    fd_set fdset;//收集描述符的集合
    while( 1 )
    {
        FD_ZERO(&fdset);
        int maxfd = -1;

        for(int i = 0; i < MAXFD; i++ )//循环将所有要关注的描述符添加到select的集合中
        {
            if( fds[i] == -1)
            {
                continue;
            }

            FD_SET(fds[i],&fdset);//将有效描述符添加到集合fdset
            if( maxfd < fds[i])
            {
                maxfd = fds[i];
            }
        }

        struct timeval tv = {5,0};

        int n = select(maxfd+1,&fdset,NULL,NULL,&tv);//阻塞，等待事件发生
        if( -1 == n )
        {
            printf("select err\n");
        }
        else if( 0 == n)
        {
            printf("time out\n");
        }
        else// n>0, 至少有一个描述符就绪
        {
            for(int i = 0; i < MAXFD; i++ )
            {
                if( -1 == fds[i])//跳过无效描述符-1
                {
                    continue;
                }

                if( FD_ISSET(fds[i],&fdset))//测试描述符是否有事件
                {
                    if( fds[i] == sockfd)
                    {
                        //accetp
                        accept_client(fds[i],fds);//接受新客户端的连接
                    }
                    else
                    {
                        //recv
                        recv_data(fds[i],fds);//接收客户端发过来的数据
                    }
                }
            }
        }
    }
    
}
int socket_init()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if( -1 == sockfd)
    {
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if( res == -1 )
    {
        printf("bind err\n");
        return -1;
    }

    res = listen(sockfd,5);
    if( -1 == res )
    {
        return -1;
    }

    return sockfd;
}