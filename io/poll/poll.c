#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

#define  MAXFD 10

int socket_init();

void fds_init(struct pollfd fds[])
{
    for(int i = 0; i < MAXFD; i++ )
    {
        fds[i].fd = -1;
        fds[i].events = 0;
        fds[i].revents = 0;
    } 
}

void fds_add(struct pollfd fds[],int fd)
{
    for(int i = 0; i < MAXFD; i++ )
    {
        if( fds[i].fd == -1)
        {
            fds[i].fd = fd;
            fds[i].events = POLLIN;// ...0000, #define POLLIN  0001,  events = POLLIN | POLLOUT  0011
            break;
        }
    }
}

void fds_del(struct pollfd fds[], int fd)
{
    for(int i = 0; i < MAXFD; i++)
    {
        if( fds[i].fd == fd)
        {
            fds[i].fd = -1;
            fds[i].events = 0;
            fds[i].revents = 0;
            break;
        }
    }
}

void accept_client(struct pollfd fds[], int sockfd)
{
    int c = accept(sockfd,NULL,NULL);
    if( c < 0)
    {
        return ;
    }

    printf("accept c=%d\n",c);

    fds_add(fds,c);
}

void recv_data(struct pollfd fds[], int c)
{
    char buff[128] = {0};
    int n = recv(c,buff,127,0);
    if( n <= 0 )
    {
        close(c);
        fds_del(fds,c);
        printf("client close(%d)\n",c);
        return ;
    }

    printf("recv(%d)=%s\n",c,buff);
    send(c,"ok",2,0);
}
int main()
{
    int sockfd = socket_init();
    if( -1 == sockfd )
    {
        exit(1);
    }

    struct pollfd fds[MAXFD];//收集用户关心的描述符和事件
    fds_init(fds);//置空
    fds_add(fds,sockfd);//events

    while( 1 )
    {
        int n = poll(fds,MAXFD,5000);//阻塞, 
        if( n == -1)
        {
            printf("poll err\n");
        }
        else if ( n == 0 )
        {
            printf("time out\n");
        }
        else//说明有n个描述符 数据就绪
        {
            for(int i = 0; i < MAXFD; i++ )
            {
                if( fds[i].fd == -1)
                {
                    continue;
                }

                if( fds[i].revents & POLLIN )//测试描述符上有没有读事件
                {
                    if ( fds[i].fd == sockfd)
                    {
                        accept_client(fds,sockfd);
                    }
                    else
                    {
                        recv_data(fds,fds[i].fd);
                    }
                }
            }
        }
    }






}
int socket_init()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(-1 == sockfd )
    {
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if( -1 == res )
    {
        printf("bind err\n");
        return -1;
    }

    res = listen(sockfd,5);
    if( -1 == res)
    {
        return -1;
    }

    return sockfd;
}