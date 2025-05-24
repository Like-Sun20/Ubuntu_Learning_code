#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#define MAXFD 10

int socket_init();

void epoll_add(int epfd, int fd) // 向内核事件表（红黑树)插入描述符（节点）
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN; // 读事件

    if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
        printf("epoll add err\n");
    }
}

void epoll_del(int epfd, int fd) // 从内核事件表（红黑树）删除节点
{
    if (epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL) == -1)
    {
        printf("epoll del err\n");
    }
}

void accept_client(int epfd, int sockfd )
{
    int c = accept(sockfd,NULL,NULL);
    if( c < 0 )
    {
        return;
    }

    printf("accept c=%d\n",c);

    epoll_add(epfd,c);
}

void recv_data(int epfd, int c)
{
    char buff[128] = {0};
    int n = recv(c,buff,127,0);
    if( n <= 0 )
    {
        epoll_del(epfd,c);
        close(c);
        printf("client close(%d)\n",c);
        return;
    }

    printf("recv(%d)=%s\n",c,buff);
    send(c,"ok",2,0);
}

int main()
{
    int sockfd = socket_init();
    if (-1 == sockfd)
    {
        exit(1);
    }

    int epfd = epoll_create(MAXFD); // 创建内核事件表--红黑树， 【就绪队列】
    if (-1 == epfd)
    {
        exit(1);
    }

    epoll_add(epfd, sockfd);

    struct epoll_event evs[MAXFD]; // 收集就绪描述符

    while (1)
    {
        int n = epoll_wait(epfd, evs, MAXFD, 5000); // 阻塞
        if (n == -1)
        {
            printf("epoll err\n");
        }
        else if (n == 0)
        {
            printf("time out\n");
        }
        else
        {
            for (int i = 0; i < n; i++)
            {
                if( evs[i].events & EPOLLIN )
                {
                    if( evs[i].data.fd == sockfd)
                    {
                        accept_client(epfd,sockfd);
                    }
                    else
                    {
                        recv_data(epfd,evs[i].data.fd);
                    }
                }

                //if( evs[i].events & EPOLLOUT)

            }
        }
    }
}

int socket_init()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == sockfd)
    {
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int res = bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));
    if (-1 == res)
    {
        printf("bind err\n");
        return -1;
    }

    res = listen(sockfd, 5);
    if (-1 == res)
    {
        return -1;
    }

    return sockfd;
}

                        